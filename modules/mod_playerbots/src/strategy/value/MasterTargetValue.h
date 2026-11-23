// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef _PLAYERBOT_MASTERTARGETVALUE_H
#define _PLAYERBOT_MASTERTARGETVALUE_H

#include "Value.h"

class PlayerbotAI;
class Unit;

class MasterTargetValue : public UnitCalculatedValue
{
public:
    MasterTargetValue(PlayerbotAI* botAI, std::string const name = "master target") : UnitCalculatedValue(botAI, name)
    {
    }

    Unit* Calculate() override;
};

#endif
