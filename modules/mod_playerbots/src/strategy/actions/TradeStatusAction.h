// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 *
 * Ported from AC mod-playerbots Ai/Base/Actions/TradeStatusAction.{h,cpp} for issue #193 (S7 of epic
 * #163). See TradeStatusAction.cpp for the full list of 5.4.8/this-fork adaptations.
 */

#ifndef PLAYERBOTS_TRADESTATUSACTION_H
#define PLAYERBOTS_TRADESTATUSACTION_H

#include "InventoryAction.h"

class Player;
class PlayerbotAI;

class TradeStatusAction : public InventoryAction
{
public:
    TradeStatusAction(PlayerbotAI* botAI) : InventoryAction(botAI, "accept trade") {}

    bool Execute(Event event) override;

private:
    void BeginTrade();
    void CancelTrade();
    bool CheckTrade();
    int32 CalculateCost(Player* player, bool sell);
};

#endif
