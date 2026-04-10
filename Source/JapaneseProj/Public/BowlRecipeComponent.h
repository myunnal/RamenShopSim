#pragma once

#include "CoreMinimal.h"
#include "OrderRecipe.h"
#include "BowlRecipeComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JAPANESEPROJ_API UBowlRecipeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TMap<EItemType, int32> CurrentIngredients;

	//Adds ingredient amount according to type
	UFUNCTION(BlueprintCallable)
	void AddIngredient(EItemType const Type, int32 const Amount);

	//Checks if the ingredients match exactly what the costumer order is
	UFUNCTION(BlueprintCallable)
	bool DoesMealMatchRecipe(FOrderRecipe CustomerRecipe);
};
