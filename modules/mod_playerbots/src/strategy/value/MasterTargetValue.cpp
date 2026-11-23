// SPDX-License-Identifier: GPL-3.0-or-later
#include "MasterTargetValue.h"

#include "Playerbots.h"

Unit* MasterTargetValue::Calculate() { return botAI->GetGroupMaster(); }
