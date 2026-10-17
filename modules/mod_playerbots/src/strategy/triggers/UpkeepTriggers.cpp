#include "UpkeepTriggers.h"

#include "Playerbots.h"

bool CanRepairTrigger::IsActive() { return !bot->IsInCombat() && AI_VALUE(bool, "can repair"); }

bool MountedTrigger::IsActive() { return bot->IsMounted() && !bot->IsInFlight(); }

bool CanSellTrigger::IsActive() { return !bot->IsInCombat() && AI_VALUE(bool, "can sell"); }
