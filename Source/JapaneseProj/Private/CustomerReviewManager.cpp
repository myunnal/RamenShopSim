#include "CustomerReviewManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogCustomerReviewManager, Log, All);

void UCustomerReviewManager::AddReview(int32 Rating)
{
	Rating = FMath::Clamp(Rating, 1, 5);

	Reviews.Add(Rating);

	if (Reviews.Num() > 15)
	{
		Reviews.RemoveAt(0);
	}
}

float UCustomerReviewManager::GetAverageRating() const
{
	float ReviewSum = 0;
	if (Reviews.Num() == 0)
	{
		return 5.0f;
	}
	
	for (const int32 Rating : Reviews)
	{
		ReviewSum += Rating;
	}

	const float AverageRating =  ReviewSum / Reviews.Num();

	return AverageRating;
}

float UCustomerReviewManager::GetPriceMultiplier() const
{
	float Rating = GetAverageRating();

	if (Rating >= 4.5f)
	{
		return 1.0f;
	}
	else if (Rating >= 4.0f)
	{
		return 0.80f;
	}
	else if (Rating >= 3.0f)
	{
		return 0.70f;
	}
	else if (Rating >= 2.0f)
	{
		return 0.60f;
	}

	return 0.50f;
}