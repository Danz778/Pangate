// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * AC parity (#408): ported from AC mod-playerbots Ai/Class/Warrior/Strategy/WarriorPullStrategy.cpp.
 *
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option) any later version.
 */

#include "WarriorPullStrategy.h"

#include "AiObjectContext.h"
#include "Player.h"
#include "PlayerbotAI.h"

std::string WarriorPullStrategy::GetPullActionName() const
{
    Unit* target = GetTarget();
    if (!target)
        return PullStrategy::GetPullActionName();

    // #408: "heroic throw" has no action registered on this fork (AC registers it in its class
    // ActionContext). PullAction::Execute() dispatches GetPullActionName() through DoSpecificAction(), so
    // resolve the action as well, otherwise the pull would name an action the bot cannot run.
    if (botAI->CanCastSpell("heroic throw", target) && botAI->GetAiObjectContext()->GetAction("heroic throw"))
        return "heroic throw";

    return PullStrategy::GetPullActionName();
}
