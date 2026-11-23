// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * This file is part of the Legends of Azeroth Pandaria Project. See THANKS file for Copyright information
 * Licensed under the GNU General Public License v3.0 or later; see COPYING.md for details.
 *
 * Ported from AC mod-playerbots Ai/Base/Actions/ResetAiAction.h (#177): the "reset botAI" chat command.
 */

#ifndef _PLAYERBOT_RESETAIACTION_H
#define _PLAYERBOT_RESETAIACTION_H

#include "Action.h"

class PlayerbotAI;

class ResetAiAction : public Action
{
public:
    ResetAiAction(PlayerbotAI* botAI) : Action(botAI, "reset botAI") {}

    bool Execute(Event event) override;
};

#endif
