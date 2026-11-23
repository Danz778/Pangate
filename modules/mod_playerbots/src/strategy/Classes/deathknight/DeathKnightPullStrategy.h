// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * AC parity (#408): ported from AC mod-playerbots Ai/Class/Dk/Strategy/DeathKnightPullStrategy.h.
 *
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option) any later version.
 */

#ifndef _PLAYERBOT_DEATHKNIGHTPULLSTRATEGY_H
#define _PLAYERBOT_DEATHKNIGHTPULLSTRATEGY_H

#include "PullStrategy.h"

class DeathKnightPullStrategy : public PullStrategy
{
public:
    DeathKnightPullStrategy(PlayerbotAI* botAI) : PullStrategy(botAI, "icy touch") {}

    std::string GetPullActionName() const override;
};

#endif
