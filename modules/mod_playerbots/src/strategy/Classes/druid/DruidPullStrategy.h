// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * AC parity (#408): ported from AC mod-playerbots Ai/Class/Druid/Strategy/DruidPullStrategy.h.
 *
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option) any later version.
 */

#ifndef _PLAYERBOT_DRUIDPULLSTRATEGY_H
#define _PLAYERBOT_DRUIDPULLSTRATEGY_H

#include "PullStrategy.h"

class DruidPullStrategy : public PullStrategy
{
public:
    DruidPullStrategy(PlayerbotAI* botAI) : PullStrategy(botAI, "faerie fire", "dire bear form") {}

    std::string GetPullActionName() const override;
    std::string GetPreActionName() const override;
};

#endif
