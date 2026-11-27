// SPDX-License-Identifier: GPL-3.0-or-later
/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 3 of the License, or (at your
* option) any later version.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* ScriptData
SDName: Instance_Shattered_Halls
SD%Complete: 100
SDComment: Instance Data Scripts and functions to acquire mobs and set encounter status for use in various Shattered Halls Scripts
SDCategory: Hellfire Citadel, Shattered Halls
EndScriptData */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "shattered_halls.h"
#include "Player.h"
#include "TemporarySummon.h"

static std::vector<DoorData> const doorData =
{
    { GO_GRAND_WARLOCK_CHAMBER_DOOR_1, DATA_NETHEKURSE, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
    { GO_GRAND_WARLOCK_CHAMBER_DOOR_2, DATA_NETHEKURSE, DOOR_TYPE_PASSAGE, BOUNDARY_NONE },
};

class instance_shattered_halls : public InstanceMapScript
{
    public:
        instance_shattered_halls() : InstanceMapScript("instance_shattered_halls", 540) { }

        InstanceScript* GetInstanceScript(Map* map) const override
        {
            return new instance_shattered_halls_InstanceMapScript(map);
        }

        struct instance_shattered_halls_InstanceMapScript : public InstanceScript
        {
            instance_shattered_halls_InstanceMapScript(Map* map) : InstanceScript(map) { }

            void Initialize() override
            {
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);

                nethekurseGUID = ObjectGuid::Empty;
                omroggGUID = ObjectGuid::Empty;
                kargathGUID = ObjectGuid::Empty;
                prisonerGUID[0] = ObjectGuid::Empty;
                prisonerGUID[1] = ObjectGuid::Empty;
                prisonerGUID[2] = ObjectGuid::Empty;
                executionerGUID = ObjectGuid::Empty;
                lastArrowGUID = ObjectGuid::Empty;
                teamInInstance = 0;
                rescueTimer = 100 * MINUTE * IN_MILLISECONDS;
            }

            void OnPlayerEnter(Player* player) override
            {
                if (!teamInInstance)
                    teamInInstance = player->GetTeam();

                // Trigger rescue timer on first player enter if not already started
                if (rescueTimer == 100 * MINUTE * IN_MILLISECONDS && instance->IsHeroic())
                {
                    SetData(DATA_ENTERED_ROOM, DATA_ENTERED_ROOM);
                }
            }

            void OnCreatureCreate(Creature* creature) override
            {
                // Ensure team is set if not already
                if (!teamInInstance)
                {
                    Map::PlayerList const& players = instance->GetPlayers();
                    if (!players.isEmpty())
                        if (Player* player = players.begin()->GetSource())
                            teamInInstance = player->GetTeam();
                }

                switch (creature->GetEntry())
                {
                    case NPC_GRAND_WARLOCK_NETHEKURSE:
                        nethekurseGUID = creature->GetGUID();
                        break;
                    case NPC_WARBRINGER_OMROGG:
                        omroggGUID = creature->GetGUID();
                        break;
                    case NPC_WARCHIEF_KARGATH:
                        kargathGUID = creature->GetGUID();
                        break;
                    case NPC_SHATTERED_EXECUTIONER:
                        executionerGUID = creature->GetGUID();
                        break;
                    case NPC_RIFLEMAN_BROWNBEARD:
                    case NPC_KORAG_PROUDMANE:
                        if (teamInInstance == TEAM_ALLIANCE)
                            creature->UpdateEntry(NPC_RIFLEMAN_BROWNBEARD, ALLIANCE);
                        else
                            creature->UpdateEntry(NPC_KORAG_PROUDMANE, HORDE);
                        prisonerGUID[0] = creature->GetGUID();
                        break;
                    case NPC_CAPTAIN_ALINA:
                    case NPC_CAPTAIN_BONESHATTER:
                        if (teamInInstance == TEAM_ALLIANCE)
                            creature->UpdateEntry(NPC_CAPTAIN_ALINA, ALLIANCE);
                        else
                            creature->UpdateEntry(NPC_CAPTAIN_BONESHATTER, HORDE);
                        prisonerGUID[1] = creature->GetGUID();
                        break;
                    case NPC_PRIVATE_JACINT:
                    case NPC_SCOUT_ORGARR:
                        if (teamInInstance == TEAM_ALLIANCE)
                            creature->UpdateEntry(NPC_PRIVATE_JACINT, ALLIANCE);
                        else
                            creature->UpdateEntry(NPC_SCOUT_ORGARR, HORDE);
                        prisonerGUID[2] = creature->GetGUID();
                        break;
                    case NPC_RANDY_WHIZZLESPROCKET:
                        if (teamInInstance == TEAM_HORDE)
                            creature->UpdateEntry(NPC_DRISELLA, HORDE);
                        break;
                    case NPC_CAZAREZ:
                        if (teamInInstance == TEAM_ALLIANCE)
                            creature->UpdateEntry(NPC_GUNNY, ALLIANCE);
                        break;
                    case NPC_STONE_GUARD_STOKTON:
                        if (teamInInstance == TEAM_ALLIANCE)
                            creature->UpdateEntry(NPC_ADVANCE_SCOUT_CHADWICK, ALLIANCE);
                        break;
                    case NPC_THRALLMAR_SCRYER:
                        if (teamInInstance == TEAM_ALLIANCE)
                            creature->UpdateEntry(NPC_HONOR_HOLD_ARCHER, ALLIANCE);
                        break;
                    case NPC_THRALLMAR_INVADER:
                        if (teamInInstance == TEAM_ALLIANCE)
                            creature->UpdateEntry(NPC_HONOR_HOLD_RECON, ALLIANCE);
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) override
            {
                switch (go->GetEntry())
                {
                    case GO_GRAND_WARLOCK_CHAMBER_DOOR_1:
                    case GO_GRAND_WARLOCK_CHAMBER_DOOR_2:
                        AddDoor(go, true);
                        break;
                }
            }

            bool SetBossState(uint32 type, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                return true;
            }

            ObjectGuid GetGuidData(uint32 data) const override
            {
                switch (data)
                {
                    case DATA_NETHEKURSE:
                        return nethekurseGUID;
                    case DATA_OMROGG:
                        return omroggGUID;
                    case DATA_KARGATH:
                        return kargathGUID;
                    case DATA_PRISONER_1:
                        return prisonerGUID[0];
                    case DATA_PRISONER_2:
                        return prisonerGUID[1];
                    case DATA_PRISONER_3:
                        return prisonerGUID[2];
                    case DATA_EXECUTIONER:
                        return executionerGUID;
                    case DATA_LAST_FLAME_ARROW:
                        return lastArrowGUID;
                    default:
                        break;
                }
                return ObjectGuid::Empty;
            }

            void SetGuidData(uint32 data, ObjectGuid value) override
            {
                if (data == DATA_LAST_FLAME_ARROW)
                    lastArrowGUID = value;
            }

            void SetData(uint32 type, uint32 data) override
            {
                if (type == DATA_ENTERED_ROOM && data == DATA_ENTERED_ROOM && rescueTimer == 100 * MINUTE * IN_MILLISECONDS)
                {
                    // Cast executioner aura on all players
                    DoCastSpellOnPlayers(SPELL_KARGATHS_EXECUTIONER_1);

                    if (Creature* kargath = instance->GetCreature(kargathGUID))
                        kargath->AI()->Talk(teamInInstance == TEAM_ALLIANCE ? 3 : 4);

                    rescueTimer = 80 * MINUTE * IN_MILLISECONDS;
                    SaveToDB();
                }
            }

            void Update(uint32 diff) override
            {
                if (rescueTimer > 0 && rescueTimer < 100 * MINUTE * IN_MILLISECONDS)
                {
                    uint32 const previousTimer = rescueTimer;
                    if (rescueTimer <= diff)
                        rescueTimer = 0;
                    else
                        rescueTimer -= diff;

                    if (previousTimer > 25 * MINUTE * IN_MILLISECONDS && rescueTimer <= 25 * MINUTE * IN_MILLISECONDS)
                    {
                        DoRemoveAurasDueToSpellOnPlayers(SPELL_KARGATHS_EXECUTIONER_1);
                        DoCastSpellOnPlayers(SPELL_KARGATHS_EXECUTIONER_2);
                        if (Creature* prisoner = instance->GetCreature(prisonerGUID[0]))
                            prisoner->Kill(prisoner);
                        if (Creature* executioner = instance->GetCreature(executionerGUID))
                            executioner->RemoveLootMode(2);
                    }
                    else if (previousTimer > 15 * MINUTE * IN_MILLISECONDS && rescueTimer <= 15 * MINUTE * IN_MILLISECONDS)
                    {
                        DoRemoveAurasDueToSpellOnPlayers(SPELL_KARGATHS_EXECUTIONER_2);
                        DoCastSpellOnPlayers(SPELL_KARGATHS_EXECUTIONER_3);
                        if (Creature* prisoner = instance->GetCreature(prisonerGUID[1]))
                            prisoner->Kill(prisoner);
                    }
                    else if (previousTimer > 0 && rescueTimer == 0)
                    {
                        DoRemoveAurasDueToSpellOnPlayers(SPELL_KARGATHS_EXECUTIONER_3);
                        if (Creature* prisoner = instance->GetCreature(prisonerGUID[2]))
                            prisoner->Kill(prisoner);
                    }
                }
            }

            std::string GetSaveData() override
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "S H " << GetBossSaveData() << rescueTimer;

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(const char* strIn) override
            {
                if (!strIn)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(strIn);

                char dataHead1, dataHead2;

                std::istringstream loadStream(strIn);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'S' && dataHead2 == 'H')
                {
                    for (uint8 i = 0; i < EncounterCount; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }
                    loadStream >> rescueTimer;
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

            protected:
                ObjectGuid nethekurseGUID;
                ObjectGuid omroggGUID;
                ObjectGuid kargathGUID;
                ObjectGuid prisonerGUID[3];
                ObjectGuid executionerGUID;
                ObjectGuid lastArrowGUID;
                uint32 teamInInstance;
                uint32 rescueTimer;
        };
};

void AddSC_instance_shattered_halls()
{
    new instance_shattered_halls();
}