// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 *
 * Ported from AC mod-playerbots Ai/Base/Actions/DestroyItemAction.h: the "destroy" chat command. AC's
 * SmartDestroyItemAction (bag-space driven, not a chat command, never registered on this fork) is not ported.
 */

#ifndef _PLAYERBOT_DESTROYITEMACTION_H
#define _PLAYERBOT_DESTROYITEMACTION_H

#include "InventoryAction.h"

class FindItemVisitor;
class PlayerbotAI;

class DestroyItemAction : public InventoryAction
{
public:
    DestroyItemAction(PlayerbotAI* botAI, std::string const name = "destroy") : InventoryAction(botAI, name) {}

    bool Execute(Event event) override;

protected:
    void DestroyItem(FindItemVisitor* visitor);
};

#endif
