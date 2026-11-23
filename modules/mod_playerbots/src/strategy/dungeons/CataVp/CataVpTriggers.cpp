// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v3 license, redistribute/modify under version 3 of the License,
 * or (at your option) any later version.
 */

#include "CataVpTriggers.h"
#include "CataVpShared.h"
#include "Playerbots.h"

bool VpErtanRingTrigger::IsActive()
{
    return CataVp::ErtanRingNeeded(bot);
}

bool VpAltairusBreathTrigger::IsActive()
{
    return CataVp::BreathNeeded(bot);
}

bool VpAltairusTwisterTrigger::IsActive()
{
    return CataVp::TwisterNeeded(bot);
}

bool VpAltairusDownwindTrigger::IsActive()
{
    return CataVp::WindNeeded(bot);
}

bool VpAsaadStackTrigger::IsActive()
{
    return CataVp::StackNeeded(bot);
}

bool VpSlipstreamTrigger::IsActive()
{
    return CataVp::SlipstreamNeeded(bot);
}
