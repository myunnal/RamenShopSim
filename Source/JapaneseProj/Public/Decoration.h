// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecorationEnum.h"
#include "Decoration.generated.h"

UCLASS()
class JAPANESEPROJ_API ADecoration : public AActor
{
	GENERATED_BODY()
	
public:	
	ADecoration();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DecorationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDecorationEnum DecorationType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DecorationPrice;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

};
