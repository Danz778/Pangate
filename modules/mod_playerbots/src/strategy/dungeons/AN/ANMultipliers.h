// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v3 license, redistribute/modify under version 3 of the License,
 * or (at your option) any later version.
 */

#ifndef PLAYERBOTS_ANMULTIPLIERS_H
#define PLAYERBOTS_ANMULTIPLIERS_H

#include "Multiplier.h"

// AC Ai/Dungeon/AN/ANMultipliers.h:12
class KrikthirMultiplier : public Multiplier
{
    public:
        KrikthirMultiplier(PlayerbotAI* ai) : Multiplier(ai, "krik'thir the gatewatcher") {}

    public:
        float GetValue(Action* action) override;
};

#endif
