#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OrderRecipe.h"
#include "DishesDataAsset.generated.h"

UCLASS(BlueprintType)
class JAPANESEPROJ_API UDishesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FOrderRecipe> Dishes;
};