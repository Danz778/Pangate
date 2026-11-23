// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_BOTAIOBJECTCONTEXT_H
#define _PLAYERBOT_BOTAIOBJECTCONTEXT_H

#include "AiObjectContext.h"

class PlayerbotAI;

class BotAiObjectContext : public AiObjectContext
{
public:
    BotAiObjectContext(PlayerbotAI* botAI);
};

#endif
