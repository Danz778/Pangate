// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef _PLAYERBOT_PETTARGETVALUE_H
#define _PLAYERBOT_PETTARGETVALUE_H

#include "Value.h"

class PlayerbotAI;
class Unit;

class PetTargetValue : public UnitCalculatedValue
{
public:
    PetTargetValue(PlayerbotAI* botAI, std::string const name = "pet target") : UnitCalculatedValue(botAI, name) {}

    Unit* Calculate() override;
};

#endif
