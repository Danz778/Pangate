// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 *
 * Ported 1:1 from AC mod-playerbots Ai/Base/Trigger/FishingTriggers.cpp:9-11 (issue #412).
 */

#include "FishingTriggers.h"

#include "Playerbots.h"

bool CanFishTrigger::IsActive() { return AI_VALUE(bool, "can fish"); }

bool CanUseFishingBobberTrigger::IsActive() { return AI_VALUE(bool, "can use fishing bobber"); }
