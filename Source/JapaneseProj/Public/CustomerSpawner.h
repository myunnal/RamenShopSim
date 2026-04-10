// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomerSpawner.generated.h"

UCLASS()
class JAPANESEPROJ_API ACustomerSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ACustomerSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSpawningEnabled = true;
};
