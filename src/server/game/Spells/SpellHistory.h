// SPDX-License-Identifier: GPL-3.0-or-later
/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 3 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SPELL_HISTORY_H
#define SPELL_HISTORY_H

#include "QueryResult.h"
#include "Transaction.h"
#include "TimeValue.h"
#include <deque>

class Unit;

class SpellHistory
{
    struct ChargeData
    {
        uint32 ConsumedCharges;
        TimeValue CurrentResetTime;
        TimeValue BaseRegenTime;
    };

    struct CooldownInfo
    {
        int64  RecoveryTime;
        uint32 Category;
        int64  CategoryRecoveryTime;
    };

public:
    struct CooldownEntry
    {
        uint32 SpellId = 0;
        TimeValue CooldownEnd;
        uint32 ItemId = 0;
        uint32 CategoryId = 0;
        TimeValue CategoryEnd;
        TimeValue ProhibitionEnd;
        bool OnHold = false;
    };

    struct RppmProcEntry
    {
        TimeValue LastAttempt;
        TimeValue LastSuccess;
        WeaponAttackType AttackTyep = BASE_ATTACK;
    };

    // Spell queueing / prediction
    struct QueuedSpell
    {
        uint32 SpellId = 0;
        uint32 ItemId = 0;
        SpellCastTargets Targets;
        TimeValue QueueTime;
    };

public:
    SpellHistory(Unit* owner);

    void LoadFromDB(PreparedQueryResult&& result, PreparedQueryResult&& chargesResult);

    template <typename Owner>
    void SaveToDB(CharacterDatabaseTransaction trans);

    // Is spell can be casted
    bool IsReady(uint32 spellId, uint32 itemId = 0, bool ignoreCategoryCooldown = false) const;
    bool IsReady(SpellInfo const* spellInfo, uint32 itemId, bool ignoreCategoryCooldown) const;

    void SetCooldownOnHold(SpellInfo const* spellInfo, uint32 itemId);
    void StartCooldown(SpellInfo const* spellInfo, uint32 itemId, Spell* spell = nullptr);
    void StartCooldownOnEvent(SpellInfo const* spellInfo, uint32 itemId, Spell* spell);

    void ProhibitSpell(uint32 spellId, TimeValue cooldow);
    void AddCooldown(uint32 spellId, uint32 itemId, int64 cooldown, uint32 categroy, int64 categoryCooldown);
    void RemoveCooldown(uint32 spellId, bool send);
    void RemoveArenaSpellCooldowns(bool removeActivePetCooldowns);
    void RemoveAllCooldowns();
    template <typename Pred>
    void RemoveCooldowns(Pred pred);
    void ModifyCooldown(uint32 spellId, int32 cooldown);
    void SendCooldown(uint32 spellId, int32 cooldown);
    void SendCooldownEvent(SpellInfo const* spellInfo, uint32 itemId = 0, Spell* spell = nullptr);

    uint32 GetCooldownDelay(uint32 spellId, uint32 itemId = 0) const;
    TimeValue GetChargeCooldown(uint32 spellId);

    bool IsProcReady(uint32 spellId) const
    {
        auto it = _procCooldowns.find(spellId);
        return it == _procCooldowns.end() || it->second <= TimeValue::Now();
    }

    void StartProcCooldown(uint32 spellId, TimeValue const& cooldown)
    {
        auto now = TimeValue::Now();
        _procCooldowns[spellId] = now + cooldown;
    }

    void RppmProcAttempt(uint32 spellId, WeaponAttackType type);
    void RppmPorcSuccess(uint32 spellId, WeaponAttackType type);

    RppmProcEntry const* GetRppmProcEntry(uint32 spellId, WeaponAttackType itemGuid);

    void UpdateCharges();

    void SendSpellHistory() const;

    // Record a spell cast for history tracking
    void RecordCast(uint32 spellId);

    // CDC (Cooldown Reduction) procs
    // Apply cooldown reduction to a target spell based on recent casts
    void ApplyCooldownReduction(uint32 targetSpellId, uint32 percentReduction, uint32 maxReductionMs = 0);
    // Check if a CDC proc should fire based on recent cast history
    bool CheckCdcProc(uint32 triggeringSpellId, uint32 requiredRecentSpellId, uint32 windowMs);

    // Spell queueing / prediction
    void QueueSpell(uint32 spellId, uint32 itemId, SpellCastTargets const& targets);
    bool HasQueuedSpell() const { return !_spellQueue.empty(); }
    QueuedSpell DequeueNextSpell();
    void ClearSpellQueue();
    size_t GetSpellQueueSize() const { return _spellQueue.size(); }

    // Spell cast history tracking
    void ClearCastHistory();
    std::vector<uint32> GetRecentCasts(uint32 windowMs = 10000) const;
    uint32 GetLastCastSpellId() const { return _lastCastSpellId; }
    TimeValue GetLastCastTime() const { return _lastCastTime; }

    // Anti-spam cast flood detection
    uint32 GetCastRate(uint32 windowMs) const;

    void OnEncounterStart();

    Player* GetPlayer() const;

    std::map<uint32, CooldownEntry> const& GetCooldowns() const { return _cooldowns; }

    void ClearAllSpellCharges();
    void ClearChargesForSpell(uint32 spell);

private:
    bool HasCooldown(SpellInfo const* spellInfo) const;
    bool HasCharge(uint32 categoryId) const;
    bool HasCategoryCooldown(uint32 categoryId) const;
    void SendClearCooldowns(std::vector<uint32> const& cooldowns);
    void SendSpellCharges() const;
    CooldownInfo GetCooldownInfo(SpellInfo const* spellInfo, uint32 itemId) const;

private:
    Unit* _owner;
    // #924: std::map, not std::unordered_map - MSVC allocates a bucket array for every unordered_map when it is
    // constructed, and every unit carries four of these (usually empty).
    std::map<uint32, CooldownEntry> _cooldowns;
    std::map<uint32, CooldownEntry> _categoryCooldowns;
    std::map<uint32, ChargeData> _spellCharges;
    std::map<uint32, TimeValue> _procCooldowns;
    std::multimap<uint32, RppmProcEntry> _procHistory;

// Spell cast history tracking (for CDC procs, queueing, anti-spam)
struct CastRecord
{
    uint32 SpellId;
    TimeValue CastTime;
};
static const uint32 MAX_CAST_HISTORY_SIZE = 100;
std::deque<CastRecord> _castHistory;
uint32 _lastCastSpellId = 0;
TimeValue _lastCastTime;

// Spell queueing / prediction
std::deque<QueuedSpell> _spellQueue;

private:

template <typename Pred>
inline void SpellHistory::RemoveCooldowns(Pred pred)
{
    std::vector<uint32> toClear;
    for (auto it = _cooldowns.begin(); it != _cooldowns.end();)
    {
        auto& entry = it->second;
        ++it;
        if (pred(entry))
            RemoveCooldown(entry.SpellId, true);
    }
}

#endif // !SPELL_HISTORY_H
