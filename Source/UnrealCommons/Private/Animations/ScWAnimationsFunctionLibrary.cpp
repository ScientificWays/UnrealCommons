// Scientific Ways

#include "Animations/ScWAnimationsFunctionLibrary.h"

#include "Animations/ScWTypes_Animations.h"
#include "Animations/ScWAnimInstance_Handheld.h"
#include "Animations/ScWAnimInstance_FirstPerson.h"
#include "Animations/ScWAnimInstance_ThirdPerson.h"

#include "Gameplay/Characters/ScWCharacter.h"

const FName UScWAnimationsFunctionLibrary::MontageDefaultGroupName = FName(TEXT("DefaultGroup"));

//~ Begin Montages
float UScWAnimationsFunctionLibrary::GetMontagePlayLength(const UAnimMontage* InMontage, float InTimeMul, float InOptionalFallbackValue)
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

float UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndex(const UAnimMontage* InMontage, int32 InIndex, float InTimeMul, float InOptionalFallbackValue)
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

float UScWAnimationsFunctionLibrary::GetMontageSectionLengthByName(const UAnimMontage* InMontage, const FName& InName, float InTimeMul, float InOptionalFallbackValue)
{
	if (InMontage)
	{
		return UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndex(InMontage, InMontage->GetSectionIndex(InName));
	}
	else
	{
		return FMath::Max(0.0f, InOptionalFallbackValue);
	}
}
//~ End Montages

//~ Begin StaticData
void UScWAnimationsFunctionLibrary::SetCharacterDefaultStaticAnimationData(AScWCharacter* InCharacter, const FScWFirstPersonStaticAnimationData& InFirstPersonStaticAnimationData, const FScWThirdPersonStaticAnimationData& InThirdPersonStaticAnimationData, const bool bInApplyDefaultStaticAnimationData)
{
	if (UScWAnimInstance_FirstPerson* FistPersonAnimInstance = InCharacter->GetScWFirstPersonAnimInstance())
	{
		FistPersonAnimInstance->Montage_StopGroupByName(0.2f, MontageDefaultGroupName);
		FistPersonAnimInstance->SetDefaultStaticAnimationData(InFirstPersonStaticAnimationData);

		if (bInApplyDefaultStaticAnimationData)
		{
			FistPersonAnimInstance->ResetToDefaultStaticAnimationData();
		}
	}
	if (UScWAnimInstance_ThirdPerson* ThirdPersonAnimInstance = InCharacter->GetScWThirdPersonAnimInstance())
	{
		ThirdPersonAnimInstance->Montage_StopGroupByName(0.2f, MontageDefaultGroupName);
		ThirdPersonAnimInstance->SetDefaultStaticAnimationData(InThirdPersonStaticAnimationData);

		if (bInApplyDefaultStaticAnimationData)
		{
			ThirdPersonAnimInstance->ResetToDefaultStaticAnimationData();
		}
	}
}

void UScWAnimationsFunctionLibrary::ToggleCharacterHolsterStaticAnimationData(AScWCharacter* InCharacter, const bool bInUseHolster, const float InBlendTime, const bool bInFirstPerson, const bool bInThirdPerson)
{
	if (InCharacter == nullptr)
	{
		return;
	}
	if (bInFirstPerson)
	{
		if (UScWAnimInstance_FirstPerson* FirstPersonAnimInstance = InCharacter->GetScWFirstPersonAnimInstance())
		{
			FirstPersonAnimInstance->SetUseHolsterStaticAnimationData(bInUseHolster, InBlendTime);
		}
	}
	if (bInThirdPerson)
	{
		if (UScWAnimInstance_ThirdPerson* ThirdPersonAnimInstance = InCharacter->GetScWThirdPersonAnimInstance())
		{
			ThirdPersonAnimInstance->SetUseHolsterStaticAnimationData(bInUseHolster, InBlendTime);
		}
	}
	if (UScWAnimInstance_Handheld* HandheldAnimInstance = InCharacter->GetScWHandheldAnimInstance())
	{
		HandheldAnimInstance->SetUseHolsterStaticAnimationData(bInUseHolster, InBlendTime);
	}
}

void UScWAnimationsFunctionLibrary::ToggleCharacterAlternativeBasePose(AScWCharacter* InCharacter, const bool bInUseAlternative, const float InBlendTime, const bool bInFirstPerson, const bool bInThirdPerson)
{
	if (InCharacter == nullptr)
	{
		return;
	}
	if (bInFirstPerson)
	{
		if (UScWAnimInstance_FirstPerson* FirstPersonAnimInstance = InCharacter->GetScWFirstPersonAnimInstance())
		{
			FirstPersonAnimInstance->SetUseAlternativeBasePose(bInUseAlternative, InBlendTime);
		}
	}
	if (bInThirdPerson)
	{
		if (UScWAnimInstance_ThirdPerson* ThirdPersonAnimInstance = InCharacter->GetScWThirdPersonAnimInstance())
		{
			ThirdPersonAnimInstance->SetUseAlternativeBasePose(bInUseAlternative, InBlendTime);
		}
	}
	if (UScWAnimInstance_Handheld* HandheldAnimInstance = InCharacter->GetScWHandheldAnimInstance())
	{
		HandheldAnimInstance->SetUseAlternativeBasePose(bInUseAlternative, InBlendTime);
	}
}

void UScWAnimationsFunctionLibrary::ToggleCharacterEnableIdleAnimation(AScWCharacter* InCharacter, const bool bInEnable, const bool bInFirstPerson, const bool bInThirdPerson)
{
	if (InCharacter == nullptr)
	{
		return;
	}
	if (bInFirstPerson)
	{
		if (UScWAnimInstance_FirstPerson* FirstPersonAnimInstance = InCharacter->GetScWFirstPersonAnimInstance())
		{
			FirstPersonAnimInstance->SetEnableIdleAnimation(bInEnable);
		}
	}
	if (bInThirdPerson)
	{
		if (UScWAnimInstance_ThirdPerson* ThirdPersonAnimInstance = InCharacter->GetScWThirdPersonAnimInstance())
		{
			ThirdPersonAnimInstance->SetEnableIdleAnimation(bInEnable);
		}
	}
	if (UScWAnimInstance_Handheld* HandheldAnimInstance = InCharacter->GetScWHandheldAnimInstance())
	{
		HandheldAnimInstance->SetEnableIdleAnimation(bInEnable);
	}
}
//~ End StaticData
