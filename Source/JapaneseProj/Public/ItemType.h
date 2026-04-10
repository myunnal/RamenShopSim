#pragma once

#include "CoreMinimal.h"
#include "ItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Egg,
	Veggies,
	Ramen,
	Bowl
};