#include "MyGameInstance.h"
#include "CustomerReviewManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogMyGameInstance, Log, All);

void UMyGameInstance::Init()
{
	Super::Init();

	ReviewManager = NewObject<UCustomerReviewManager>(this);
}

UCustomerReviewManager* UMyGameInstance::GetReviewManager() const
{
	return ReviewManager;
}