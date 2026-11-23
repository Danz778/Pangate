// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v3 license, redistribute/modify under version 3 of the License,
 * or (at your option) any later version.
 */

// AC Ai/Dungeon/Mech/MechTriggers.cpp (#566, epic #569)
#include "MechTriggers.h"
#include "AiObject.h"
#include "MechShared.h"
#include "Playerbots.h"

bool SepethreaKiteFlameTrigger::IsActive()
{
    if (!MechanarFlames::GetSepethrea(bot))
        return false;

    if (!MechanarFlames::GetFixatingFlame(bot))
        return false;

    return !MechanarFlames::TankMustGrabBoss(bot);
}

bool SepethreaAvoidFlameTrigger::IsActive()
{
    if (!MechanarFlames::GetSepethrea(bot))
        return false;

    if (MechanarFlames::GetFixatingFlame(bot))
        return false;

    if (!MechanarFlames::IsFlameNearCached(bot))
        return false;

    return !MechanarFlames::HealerHoldsFire(bot);
}

bool SepethreaTrailTrigger::IsActive()
{
    if (!MechanarFlames::GetSepethrea(bot))
        return false;

    if (!MechanarFlames::InTrailDangerCached(bot))
        return false;

    return !MechanarFlames::HealerHoldsFire(bot);
}

bool SepethreaFocusBossTrigger::IsActive()
{
    return MechanarFlames::GetSepethrea(bot) != nullptr;
}
