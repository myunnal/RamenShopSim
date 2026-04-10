#define ECC_NO_DECORATION_ZONE ECC_GameTraceChannel2

#include "PlayerCharacter.h"

#include "Decoration.h"
#include "CustomerSpawner.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerCharacter, Log, All);


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(PreviewDecoration))
	{
		return;
	}
	
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	if (PlayerController == nullptr)
	{
		return;
	}

	if (FHitResult Hit; PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		PreviewDecoration->SetActorLocation(Hit.Location);
		const FRotator CurrentRotation = PreviewDecoration->GetActorRotation();
		PreviewDecoration->SetActorRotation(FRotator(0, CurrentRotation.Yaw, 0));
		
		UpdateForbiddenZoneColor(IsDecorationInForbiddenZone());
		
	}
}

void APlayerCharacter::RemoveAllActors() const
{
	if ((CustomerClass == nullptr) || (CustomerSpawnerClass == nullptr))
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("CustomerClass or CustomerSpawnerClass is not set"));
		return;
	}

	TArray<AActor*> FoundActors;
	TArray<AActor*> FoundSpawners;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CustomerClass, FoundActors);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CustomerSpawnerClass, FoundSpawners);
	
	for (AActor* Actor : FoundSpawners)
	{
		if (ACustomerSpawner* Spawner = Cast<ACustomerSpawner>(Actor))
		{
			Spawner->bIsSpawningEnabled = false;
		}
	}
	
	for (AActor* Actor : FoundActors)
	{
		if (Actor != nullptr)
		{
			Actor->Destroy();
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if ((PlaceDecorationsInput == nullptr) || (RotateDecorationInput == nullptr))
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("Input actions are not set"));
		return;
	}
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(PlaceDecorationsInput, ETriggerEvent::Started, this, &APlayerCharacter::PlaceDecoration);
		EnhancedInput->BindAction(RotateDecorationInput, ETriggerEvent::Started, this, &APlayerCharacter::RotateDecoration);
	}
}

void APlayerCharacter::EnterDecorationMode() const
{
	APlayerController* const PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogPlayerCharacter, Warning, TEXT("PlayerController not found"));
		return;
	}
	
	RemoveAllActors();
	
	if ((GameplayContext == nullptr) || (EditModeContext == nullptr))
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("Input Mapping Contexts not set"));
		return;
	}
	
	PlayerController->bShowMouseCursor = true;	
	PlayerController->bEnableClickEvents = true;
	PlayerController->SetInputMode(FInputModeGameAndUI());
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(GameplayContext);
		Subsystem->AddMappingContext(EditModeContext, 1);
	}
	else
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("EnhancedInputSubsystem is null"));
	}
}

void APlayerCharacter::ExitDecorationMode() const
{
	APlayerController* const PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController)
    {
    	UE_LOG(LogPlayerCharacter, Warning, TEXT("PlayerController not found"));
    	return;
    }
    
    if ((GameplayContext == nullptr) || (EditModeContext == nullptr))
    	{
    		UE_LOG(LogPlayerCharacter, Error, TEXT("Input Mapping Contexts not set"));
    		return;
    	}
    	PlayerController->SetInputMode(FInputModeUIOnly());
    	
    	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    	{
    		Subsystem->RemoveMappingContext(EditModeContext);
    		Subsystem->AddMappingContext(GameplayContext, 1);
    	}
    	else
    	{
    		UE_LOG(LogPlayerCharacter, Error, TEXT("EnhancedInputSubsystem is null"));
    	}	
    
}

bool APlayerCharacter::CanPlaceDecoration() const
{
	if (!IsValid(PreviewDecoration))
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("PreviewDecoration is invalid"));
		return false;
	}

	if (IsDecorationInForbiddenZone())
	{
		return false;
	}

	return true;
}

bool APlayerCharacter::IsDecorationInForbiddenZone() const
{
	if ((PreviewDecoration == nullptr) || (GetWorld() == nullptr))
	{
		return false;
	}

	const UPrimitiveComponent* PrimitiveComponent = PreviewDecoration->FindComponentByClass<UPrimitiveComponent>();
	if (PrimitiveComponent == nullptr)
	{
		return false;
	}

	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PreviewDecoration);

	const bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		PreviewDecoration->GetActorLocation(),
		PreviewDecoration->GetActorQuat(),
		FCollisionObjectQueryParams(ECC_NO_DECORATION_ZONE),
		PrimitiveComponent->GetCollisionShape(),
		QueryParams
	);
	
	return  bHasOverlap;
}

void APlayerCharacter::UpdateForbiddenZoneColor(const bool bIsForbiddenZone) const
{
	if (DecorationPreviewMaterial == nullptr)
	{
		return;
	}

	if (bIsForbiddenZone)
	{
		DecorationPreviewMaterial ->SetVectorParameterValue(TEXT("DecorationPreviewColor"), FLinearColor::Red);
	}
	else
	{
		DecorationPreviewMaterial ->SetVectorParameterValue(TEXT("DecorationPreviewColor"), FLinearColor::White);
	}
}

void APlayerCharacter::StartDecorationPlacement(TSubclassOf<ADecoration> DecorationClass)
{
	if (DecorationClass == nullptr)
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("DecorationClass is null"));
		return;
	}
	
	if (GetWorld() == nullptr)
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("World is null"));
		return;
	}

	SelectedDecoration = DecorationClass;

	if  (PreviewDecoration != nullptr)
	{
		PreviewDecoration->Destroy();
		DecorationPreviewMaterial  = nullptr;
	}
	
	const FVector InitialSpawnLocation = GetActorLocation();
	
	PreviewDecoration = GetWorld()->SpawnActor<ADecoration>(SelectedDecoration, InitialSpawnLocation, FRotator::ZeroRotator);
	
	if (PreviewDecoration)
	{
		PreviewDecoration->SetActorEnableCollision(false);

		if (UStaticMeshComponent* StaticMeshComp = PreviewDecoration->FindComponentByClass<UStaticMeshComponent>())
		{
			DecorationPreviewMaterial = StaticMeshComp->CreateAndSetMaterialInstanceDynamic(0);
		}
	}
	else
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("Failed to spawn the preview decoration!"));
	}
	
}

void APlayerCharacter::PlaceDecoration()
{
	if (!CanPlaceDecoration())
	{
		return;
	}
	
	if (UPrimitiveComponent* RootPrim = PreviewDecoration->FindComponentByClass<UPrimitiveComponent>())
	{
		PreviewDecoration->SetActorEnableCollision(true);
		RootPrim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	
	DecorationPreviewMaterial  = nullptr;
	PreviewDecoration = nullptr;
}

void APlayerCharacter::RotateDecoration()
{
	if (!IsValid(PreviewDecoration))
	{
		return;
	}
	FRotator NewRotation = PreviewDecoration->GetActorRotation();
	NewRotation.Yaw += ItemRotation;

	PreviewDecoration->SetActorRotation(NewRotation);
	
}

