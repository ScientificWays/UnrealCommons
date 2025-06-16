// G.o.o.P Studios

#include "AnimationsFunctionLibrary.h"

//~ Begin Montages
float UAnimationsFunctionLibrary::GetMontagePlayLength(const UAnimMontage* InMontage, float InTimeMul, float InOptionalFallbackValue)
{
	if (InMontage)
	{
		return FMath::Max(InMontage->GetPlayLength() * InTimeMul, InOptionalFallbackValue);
	}
	else
	{
		return FMath::Max(0.0f, InOptionalFallbackValue);
	}
}

float UAnimationsFunctionLibrary::GetMontageSectionLengthByIndex(const UAnimMontage* InMontage, int32 InIndex, float InTimeMul, float InOptionalFallbackValue)
{
	if (InMontage)
	{
		return (!InMontage->IsValidSectionIndex(InIndex) && InOptionalFallbackValue >= 0.0f)
			? (InOptionalFallbackValue)
			: (InMontage->GetSectionLength(InIndex) * InTimeMul);
	}
	else
	{
		return FMath::Max(0.0f, InOptionalFallbackValue);
	}
}

float UAnimationsFunctionLibrary::GetMontageSectionLengthByName(const UAnimMontage* InMontage, const FName& InName, float InTimeMul, float InOptionalFallbackValue)
{
	if (InMontage)
	{
		return UAnimationsFunctionLibrary::GetMontageSectionLengthByIndex(InMontage, InMontage->GetSectionIndex(InName));
	}
	else
	{
		return FMath::Max(0.0f, InOptionalFallbackValue);
	}
}
//~ End Montages
