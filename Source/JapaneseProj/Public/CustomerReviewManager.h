#pragma once

#include "CoreMinimal.h"
#include "CustomerReviewManager.generated.h"

UCLASS(Blueprintable)
class UCustomerReviewManager : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void AddReview(int32 Rating);

	UFUNCTION(BlueprintCallable)
	float GetAverageRating() const;
	
	UFUNCTION(BlueprintCallable)
	float GetPriceMultiplier() const;

private:

	UPROPERTY()
	TArray<int32> Reviews;
};
