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

#ifndef DEF_SHATTERED_H
#define DEF_SHATTERED_H

uint32 const EncounterCount          = 3;

enum DataTypes
{
    DATA_NETHEKURSE                  = 1,
    DATA_OMROGG                      = 2,
    DATA_KARGATH                     = 3,
    DATA_ENTERED_ROOM                = 10,
    DATA_PRISONER_1                  = 11,
    DATA_PRISONER_2                  = 12,
    DATA_PRISONER_3                  = 13,
    DATA_EXECUTIONER                 = 14,
    DATA_LAST_FLAME_ARROW            = 18
};

enum SpellIds
{
    SPELL_KARGATHS_EXECUTIONER_1    = 39288,
    SPELL_KARGATHS_EXECUTIONER_2    = 39289,
    SPELL_KARGATHS_EXECUTIONER_3    = 39290
};

enum CreatureIds
{
    NPC_GRAND_WARLOCK_NETHEKURSE = 16807,
    NPC_WARCHIEF_KARGATH         = 16808,
    NPC_WARBRINGER_OMROGG        = 16809,
    NPC_SHATTERED_EXECUTIONER    = 17301,

    // Alliance
    NPC_GUNNY                    = 54934,
    NPC_ADVANCE_SCOUT_CHADWICK   = 54933,
    NPC_HONOR_HOLD_ARCHER        = 54935,
    NPC_HONOR_HOLD_RECON         = 57907,
    NPC_RANDY_WHIZZLESPROCKET    = 17288,
    NPC_RIFLEMAN_BROWNBEARD      = 17289,
    NPC_CAPTAIN_ALINA            = 17290,
    NPC_PRIVATE_JACINT           = 17292,

    // Horde
    NPC_CAZAREZ                  = 54932,
    NPC_STONE_GUARD_STOKTON      = 54931,
    NPC_THRALLMAR_SCRYER         = 57908,
    NPC_THRALLMAR_INVADER        = 57909,
    NPC_DRISELLA                 = 17294,
    NPC_KORAG_PROUDMANE          = 17295,
    NPC_CAPTAIN_BONESHATTER      = 17296,
    NPC_SCOUT_ORGARR             = 17297,
};

enum GameobjectIds
{
    GO_GRAND_WARLOCK_CHAMBER_DOOR_1  = 182539,
    GO_GRAND_WARLOCK_CHAMBER_DOOR_2  = 182540
};

#endif
