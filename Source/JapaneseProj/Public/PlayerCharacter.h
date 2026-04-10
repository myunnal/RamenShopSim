#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Decoration.h"
#include "EnhancedInputSubsystems.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class JAPANESEPROJ_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	
	UPROPERTY()
	ADecoration* PreviewDecoration;

	UPROPERTY()
	TSubclassOf<ADecoration> SelectedDecoration;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* GameplayContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* EditModeContext;
	
	UPROPERTY(EditAnywhere)
	UInputAction* PlaceDecorationsInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* RotateDecorationInput;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DecorationPreviewMaterial  = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor InvalidPlacementColor = FLinearColor(1.f, 0.f, 0.f, 0.6f);
	
	//Spawns a decoration item and allows the user to drag it by the mouse
	UFUNCTION(BlueprintCallable)
	void StartDecorationPlacement(TSubclassOf<ADecoration> DecorationClass);
	
	//Opens the decoration mode where the player can decorate the restoraunt environment, removes unnecessary actors
	UFUNCTION(BlueprintCallable)
	void EnterDecorationMode() const;
	
	UFUNCTION(BlueprintCallable)
	void ExitDecorationMode() const;
	
	//Places decorations in world when clicked
	UFUNCTION(BlueprintCallable)
	void PlaceDecoration();
	
	//Rotates the spawned decoration
	UFUNCTION(BlueprintCallable)
	void RotateDecoration();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> CustomerClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> CustomerSpawnerClass;

	UPROPERTY(EditAnywhere)
	float ItemRotation = 90.0f;
	
	
private:

	//Removes all spawned customer actors and the spawners
	void RemoveAllActors() const;
	
	//Sets up the editor enhanced input for decoration placement
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	//Checks if the decoration actor is overlapping the forbidden zone Blocking Volumes
	bool IsDecorationInForbiddenZone() const;
	
	//Checks if the player can place a decoration 
	bool CanPlaceDecoration() const;
	
	//Updates the color regarding if the item is in a forbidden zone or not
	void UpdateForbiddenZoneColor(bool bIsForbiddenZone) const;
	
};
