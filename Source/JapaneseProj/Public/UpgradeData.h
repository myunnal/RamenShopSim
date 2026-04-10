// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UpgradeTypesEnnum.h"
#include "UpgradeData.generated.h"

USTRUCT(BlueprintType)
struct FUpgradeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EUpgradeType UpgradeType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WalkSpeedBonus;
	
};