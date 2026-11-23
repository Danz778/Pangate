// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * AC parity (#408): ported from AC mod-playerbots Ai/Class/Paladin/Strategy/PaladinPullStrategy.h.
 *
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option) any later version.
 */

#ifndef _PLAYERBOT_PALADINPULLSTRATEGY_H
#define _PLAYERBOT_PALADINPULLSTRATEGY_H

#include "PullStrategy.h"

class PaladinPullStrategy : public PullStrategy
{
public:
    PaladinPullStrategy(PlayerbotAI* botAI) : PullStrategy(botAI, "judgement", "seal of righteousness") {}

    std::string GetPullActionName() const override;
    std::string GetPreActionName() const override;
};

#endif
