// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UpgradeTypesEnnum.h"
#include "Engine/GameInstance.h"
#include "CustomerReviewManager.h"
#include "MyGameInstance.generated.h"

UCLASS()
class JAPANESEPROJ_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	virtual void Init() override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<TSubclassOf<AActor>> SavedActorClasses;

	UPROPERTY(BlueprintReadWrite)
	TArray<FTransform> SavedActorTransforms;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<EUpgradeType> PurchasedUpgrades;
	
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentCurrency;
	
	UFUNCTION(BlueprintCallable)
	UCustomerReviewManager* GetReviewManager() const;
	
private:

	UPROPERTY()
	UCustomerReviewManager* ReviewManager;
};
