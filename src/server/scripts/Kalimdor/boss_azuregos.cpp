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
* FITNESS FOR A PARTICULAR PURPOSE. See GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* ScriptData
SDName: Boss_Azuregos
SD%Complete: 90
SDComment: Teleport not included, spell reflect not effecting dots (Core problem)
SDCategory: Azshara
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Say
{
    SAY_AGGRO               = 0,
    SAY_TELEPORT            = 1,
    SAY_REFLECT             = 2,
    SAY_KILL                = 3
};

enum Spells
{
    SPELL_MARK_OF_FROST     = 23182, // Applied on player death
    SPELL_AURA_OF_FROST     = 23186, // Freeze aura if player has Mark of Frost
    SPELL_MANASTORM         = 21097, // AoE health and mana drain
    SPELL_CHILL             = 21098, // Movement and attack speed slow
    SPELL_FROST_BREATH      = 21099, // Frontal cone frost damage
    SPELL_REFLECT           = 22067, // Magic reflection shield
    SPELL_CLEAVE            = 19983, // Frontal cleave
    SPELL_ARCANE_VACUUM     = 21147, // Teleports nearby players & wipes threat
    SPELL_ENRAGE            = 23537  // Enrage
};

enum Events
{
    EVENT_FROST_BREATH      = 1,
    EVENT_CLEAVE            = 2,
    EVENT_CHILL             = 3,
    EVENT_MANASTORM         = 4,
    EVENT_REFLECT           = 5,
    EVENT_ARCANE_VACUUM     = 6
};

class boss_azuregos : public CreatureScript
{
public:
    boss_azuregos() : CreatureScript("boss_azuregos") { }

    struct boss_azuregosAI : public ScriptedAI
    {
        boss_azuregosAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            _events.Reset();
            _enraged = false;
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            Talk(SAY_AGGRO);

            _events.ScheduleEvent(EVENT_FROST_BREATH, 12s);
            _events.ScheduleEvent(EVENT_CLEAVE, 7s);
            _events.ScheduleEvent(EVENT_CHILL, 15s);
            _events.ScheduleEvent(EVENT_MANASTORM, 18s);
            _events.ScheduleEvent(EVENT_REFLECT, 25s);
            _events.ScheduleEvent(EVENT_ARCANE_VACUUM, 30s);
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
            {
                Talk(SAY_KILL);
                // Apply Mark of Frost so players cannot immediately corpse-run back
                victim->CastSpell(victim, SPELL_MARK_OF_FROST, true);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FROST_BREATH:
                        DoCastVictim(SPELL_FROST_BREATH);
                        _events.ScheduleEvent(EVENT_FROST_BREATH, randtime(10s, 15s));
                        break;

                    case EVENT_CLEAVE:
                        DoCastVictim(SPELL_CLEAVE);
                        _events.ScheduleEvent(EVENT_CLEAVE, randtime(7s, 10s));
                        break;

                    case EVENT_CHILL:
                        DoCastVictim(SPELL_CHILL);
                        _events.ScheduleEvent(EVENT_CHILL, randtime(13s, 20s));
                        break;

                    case EVENT_MANASTORM:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                            DoCast(target, SPELL_MANASTORM);
                        _events.ScheduleEvent(EVENT_MANASTORM, randtime(15s, 25s));
                        break;

                    case EVENT_REFLECT:
                        Talk(SAY_REFLECT);
                        DoCast(me, SPELL_REFLECT);
                        _events.ScheduleEvent(EVENT_REFLECT, randtime(25s, 35s));
                        break;

                    case EVENT_ARCANE_VACUUM:
                        Talk(SAY_TELEPORT);
                        DoCastAOE(SPELL_ARCANE_VACUUM);
                        TeleportAndWipeThreat();
                        _events.ScheduleEvent(EVENT_ARCANE_VACUUM, randtime(30s, 40s));
                        break;

                    default:
                        break;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            if (HealthBelowPct(26) && !_enraged)
            {
                DoCast(me, SPELL_ENRAGE);
                _enraged = true;
            }

            DoMeleeAttackIfReady();
        }

    private:
        void TeleportAndWipeThreat()
        {
            auto threatList = me->GetThreatManager().GetUnsortedThreatList();
            for (ThreatReference const* ref : threatList)
            {
                if (Unit* unit = ref->GetVictim())
                {
                    if (unit->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(unit, 30.0f))
                    {
                        DoTeleportPlayer(unit, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 2.0f, unit->GetOrientation());
                    }
                }
            }
            DoResetThreat();
        }

        EventMap _events;
        bool _enraged = false;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_azuregosAI(creature);
    }
};

void AddSC_boss_azuregos()
{
    new boss_azuregos();
}
