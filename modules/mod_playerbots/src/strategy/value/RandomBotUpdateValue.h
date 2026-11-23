// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef _PLAYERBOT_RANDOMBOTUPDATEVALUE_H
#define _PLAYERBOT_RANDOMBOTUPDATEVALUE_H

#include "Value.h"

class PlayerbotAI;

class RandomBotUpdateValue : public ManualSetValue<bool>
{
public:
    RandomBotUpdateValue(PlayerbotAI* botAI, std::string const name = "random bot update")
        : ManualSetValue<bool>(botAI, false, name)
    {
    }
};

#endif
