// SPDX-License-Identifier: GPL-3.0-or-later
#include "PetTargetValue.h"

#include "Playerbots.h"

Unit* PetTargetValue::Calculate() { return botAI->GetBot()->GetPet(); }
