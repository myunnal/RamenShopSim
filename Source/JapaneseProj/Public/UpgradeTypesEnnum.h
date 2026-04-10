#pragma once

#include "CoreMinimal.h"
#include "UpgradeTypesEnnum.generated.h"

UENUM(BlueprintType)
enum class EUpgradeType : uint8		
{
	WalkSpeed,
	ExtraRamenCooker,
	ExtraCuttingBoard,
	ExtraEggBoiler
};