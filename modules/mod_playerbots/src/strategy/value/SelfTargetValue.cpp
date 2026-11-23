// SPDX-License-Identifier: GPL-3.0-or-later
#include "SelfTargetValue.h"

#include "Playerbots.h"

Unit* SelfTargetValue::Calculate() { return botAI->GetBot(); }