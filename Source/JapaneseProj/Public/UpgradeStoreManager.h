#pragma once

#include "CoreMinimal.h"
#include "MyGameInstance.h"
#include "Components/ActorComponent.h"
#include "UpgradeData.h"
#include "UpgradeStoreManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JAPANESEPROJ_API UUpgradeStoreManager : public UActorComponent
{
	GENERATED_BODY()

public:
	//Array of Upgrades that are available for the player to buy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	TArray<FUpgradeData> AvailableUpgrades;
	
	UPROPERTY(EditAnywhere, Category="Upgrade")
	TSubclassOf<AActor> NoodleCooker;
	
	UPROPERTY(EditAnywhere, Category="Upgrade")
	TSubclassOf<AActor> ChoppingArea;
	
	//Noodle Cooker spawn location
	UPROPERTY(EditAnywhere, Category="Upgrade Spawns")
	FTransform NoodleCookerSpawn;
	
	//Cutting Board spawn location
	UPROPERTY(EditAnywhere, Category="Upgrade Spawns")
	FTransform CuttingBoardSpawn;
	
	//Gets the player's currency from the game instance
	UFUNCTION(BlueprintCallable, Category="Currency")
	int32 GetCurrency() const;
	
	//Adds currency
	UFUNCTION(BlueprintCallable, Category="Currency")
	void AddCurrency(int32 const Amount);

	//Purchases an upgrade if the player has enough money and hasn't bought it yet
	UFUNCTION(BlueprintCallable, Category="Upgrade")
	bool BuyUpgrade(EUpgradeType UpgradeType);

private:
	//Checks for available updates and gets the data for
	bool GetUpgradeData(EUpgradeType const UpgradeType, FUpgradeData& UpgradeData);
	//Applies a specific upgrade
	void ApplyUpgrade(const FUpgradeData& Data) const; 
	//Applies a speed boost to the player character
	void ApplyWalkSpeed(float SpeedIncrease) const;
	//Spawns a noodle cooker at specific coordinates 
	void SpawnNoodleCooker() const;
	//Spawns a cutting board at specific coordinates 
	void SpawnCuttingBoard() const;
	//Casts to MyGameInstance
	UMyGameInstance* GetMyGameInstance() const;
};