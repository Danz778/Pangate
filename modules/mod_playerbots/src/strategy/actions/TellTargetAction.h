// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 *
 * Ported from AC mod-playerbots Ai/Base/Actions/TellTargetAction.h: the "target" and "attackers" chat commands.
 */

#ifndef _PLAYERBOT_TELLTARGETACTION_H
#define _PLAYERBOT_TELLTARGETACTION_H

#include "Action.h"

class PlayerbotAI;

class TellTargetAction : public Action
{
public:
    TellTargetAction(PlayerbotAI* botAI) : Action(botAI, "tell target") {}

    bool Execute(Event event) override;
};

class TellAttackersAction : public Action
{
public:
    TellAttackersAction(PlayerbotAI* botAI) : Action(botAI, "tell attackers") {}

    bool Execute(Event event) override;
};

#endif
