#include "UpgradeStoreManager.h"
#include "GameFramework/Character.h"
#include "MyGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogUpgradeStoreManager, Log, All);

UMyGameInstance* UUpgradeStoreManager::GetMyGameInstance() const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogUpgradeStoreManager, Error, TEXT("World is null"));
		return nullptr;
	}

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(World->GetGameInstance());
	UE_CLOG(!GameInstance, LogUpgradeStoreManager, Error, TEXT("GameInstance cast failed"));

	return GameInstance;
}

int32 UUpgradeStoreManager::GetCurrency() const
{
	if (const UMyGameInstance* GameInstance = GetMyGameInstance())
	{
		return GameInstance->CurrentCurrency;
	}
	
	UE_LOG(LogUpgradeStoreManager, Warning, TEXT("GameInstance missing when getting currency"));
	return 0;
}

bool UUpgradeStoreManager::BuyUpgrade(const EUpgradeType UpgradeType)
{
	UMyGameInstance* GameInstance = GetMyGameInstance();
	if (!GameInstance)
	{
		return false;
	}
	
	if (GameInstance->PurchasedUpgrades.Contains(UpgradeType))
	{
		UE_LOG(LogUpgradeStoreManager, Warning, TEXT("Attempted to purchase already owned upgrade"));
		return false;
	}
	
	FUpgradeData Data;
	if (!GetUpgradeData(UpgradeType, Data))
	{
        UE_LOG(LogUpgradeStoreManager, Error, TEXT("Upgrade data missing"));
		return false;
	}
	
	if (GameInstance->CurrentCurrency < Data.Cost)
	{
		return false;
	}
	
	GameInstance->CurrentCurrency -= Data.Cost;
	
	ApplyUpgrade(Data);

	GameInstance->PurchasedUpgrades.Add(UpgradeType);
	
	return true;
}

void UUpgradeStoreManager::AddCurrency(int32 const Amount)
{
	if (UMyGameInstance* GameInstance = GetMyGameInstance())
	{
		GameInstance->CurrentCurrency += Amount;
	}
}

bool UUpgradeStoreManager::GetUpgradeData(EUpgradeType const UpgradeType, FUpgradeData& UpgradeData) 
{
	for (const FUpgradeData& Upgrade : AvailableUpgrades)
	{
		if (Upgrade.UpgradeType == UpgradeType)
		{
			UpgradeData = Upgrade;
			return true;
		}
	}
	return false;
}

void UUpgradeStoreManager::ApplyUpgrade(const FUpgradeData& Data) const
{
	switch (Data.UpgradeType)
	{
	case EUpgradeType::WalkSpeed:
		ApplyWalkSpeed(Data.WalkSpeedBonus);
		break;

	case EUpgradeType::ExtraRamenCooker:
		SpawnNoodleCooker();
		break;

	case EUpgradeType::ExtraCuttingBoard:
		SpawnCuttingBoard();
		break;

	case EUpgradeType::ExtraEggBoiler:
		break;
		
	default:
		UE_LOG(LogUpgradeStoreManager, Warning, TEXT("Unknown upgrade"));
		break;
	}
}

void UUpgradeStoreManager::ApplyWalkSpeed(const float SpeedIncrease) const
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!IsValid(Character))
	{
		UE_LOG(LogUpgradeStoreManager, Warning, TEXT("Owner is not a valid Character"));
		return;
	}
	
	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (!MovementComponent)
	{
		UE_LOG(LogUpgradeStoreManager, Error, TEXT("The Movement Component is missing"));
		return;
	}
		
	MovementComponent->MaxWalkSpeed += SpeedIncrease;
}

void UUpgradeStoreManager::SpawnNoodleCooker() const
{
	UWorld* World = GetWorld();
	
	if (!World)
	{
		UE_LOG(LogUpgradeStoreManager, Error, TEXT("World is not valid when spawning Noodle Cooker"));
		return;
	}
	
	if (!NoodleCooker)
	{
		UE_LOG(LogUpgradeStoreManager, Error, TEXT("Noodle Cooker class is not set"));
		return;
	}
	
	const AActor* SpawnedActor = World->SpawnActor<AActor>(NoodleCooker, NoodleCookerSpawn);
	
	if (!SpawnedActor)
	{
		UE_LOG(LogUpgradeStoreManager, Error, TEXT("Failed to spawn Noodle Cooker"));
		return;
	}

	if (UMyGameInstance* GameInstance = GetMyGameInstance(); GameInstance && SpawnedActor)
	{
		GameInstance->SavedActorClasses.Add(SpawnedActor->GetClass());
		GameInstance->SavedActorTransforms.Add(SpawnedActor->GetActorTransform());
	}
}

void UUpgradeStoreManager::SpawnCuttingBoard() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogUpgradeStoreManager, Error, TEXT("World is not valid when spawning the Chopping Board"));
		return;
	}
	
	if (!ChoppingArea)
	{
		UE_LOG(LogUpgradeStoreManager, Error, TEXT("Chopping Area class is not set"));
		return;
	}
	
	const AActor* SpawnedActor = World->SpawnActor<AActor>(ChoppingArea, CuttingBoardSpawn);
	if (!SpawnedActor)
	{
		UE_LOG(LogUpgradeStoreManager, Error, TEXT("Failed to spawn the Chopping Board"));
		return;
	}

	if (UMyGameInstance* GameInstance = GetMyGameInstance(); GameInstance && SpawnedActor)
	{
		GameInstance->SavedActorClasses.Add(SpawnedActor->GetClass());
		GameInstance->SavedActorTransforms.Add(SpawnedActor->GetActorTransform());
	}
}