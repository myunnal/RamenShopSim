#pragma once

#include "CoreMinimal.h"
#include "ItemType.h"	
#include "OrderRecipe.generated.h"

USTRUCT(BlueprintType)
struct FOrderRecipe
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DishName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EItemType, int32> RequiredIngredients;
};
