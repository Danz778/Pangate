// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v3 license, redistribute/modify under version 3 of the License,
 * or (at your option) any later version.
 */

// AC Ai/Dungeon/Ramp/RampBossHelper.h (#568, epic #569)
#ifndef PLAYERBOTS_RAMPBOSSHELPER_H
#define PLAYERBOTS_RAMPBOSSHELPER_H

#include "Player.h"

class OmorBossHelper
{
public:
    bool HasTreacheryAura(Player const* bot);
};

#endif
