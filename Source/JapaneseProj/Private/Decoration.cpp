// Fill out your copyright notice in the Description page of Project Settings.


#include "Decoration.h"

ADecoration::ADecoration()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}




