#include "VendorTriggers.h"

#include "Playerbots.h"

bool CanRepairTrigger::IsActive() { return !bot->IsInCombat() && AI_VALUE(bool, "can repair"); }

bool CanSellTrigger::IsActive() { return !bot->IsInCombat() && AI_VALUE(bool, "can sell"); }
