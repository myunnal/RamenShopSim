#include "BowlRecipeComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBowlRecipeComponent, Log, All);

void UBowlRecipeComponent::AddIngredient(EItemType const Type, int32 const Amount)
{
	UE_CLOG(Amount <= 0, LogBowlRecipeComponent, Warning, TEXT("Attempted to add an invalid ingredient amount"));
	
	if (Amount <= 0)
	{
		return;
	}
	
	CurrentIngredients.FindOrAdd(Type) += Amount;
}
bool UBowlRecipeComponent::DoesMealMatchRecipe(FOrderRecipe CustomerRecipe) 
{
	if (CustomerRecipe.RequiredIngredients.Num() == 0)
	{
		UE_LOG(LogBowlRecipeComponent, Warning, TEXT("Customer recipe has no ingredients"));
		return false;
	}
	
	if (CurrentIngredients.Num() != CustomerRecipe.RequiredIngredients.Num())
	{
		return false;
	}
	
	for(const auto& [Key, Value] : CustomerRecipe.RequiredIngredients) 
	{
		if (const int32* CurrentAmountInBowl = CurrentIngredients.Find(Key); !CurrentAmountInBowl || *CurrentAmountInBowl != Value)
		{
			return false;
		}
	}
	return true;
}