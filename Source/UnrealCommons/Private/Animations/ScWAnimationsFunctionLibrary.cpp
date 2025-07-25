// Scientific Ways

#include "Animations/ScWAnimationsFunctionLibrary.h"

#include "Animations/ScWTypes_Animations.h"
#include "Animations/ScWAnimInstance_Handheld.h"
#include "Animations/ScWAnimInstance_FirstPerson.h"
#include "Animations/ScWAnimInstance_ThirdPerson.h"

#include "Gameplay/Characters/ScWCharacter.h"

const FName UScWAnimationsFunctionLibrary::MontageDefaultGroupName = FName(TEXT("DefaultGroup"));

//~ Begin Montages
float UScWAnimationsFunctionLibrary::GetMontagePlayLength(const UAnimMontage* InMontage, float InTimeMul, float InFallbackValue)
{
	ensureReturn(InMontage, FMath::Max(0.0f, InFallbackValue));
	return FMath::Max(InMontage->GetPlayLength() * InTimeMul, InFallbackValue);
}

float UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndex(const UAnimMontage* InMontage, int32 InIndex, float InTimeMul, float InFallbackValue)
{
	ensureReturn(InMontage, FMath::Max(0.0f, InFallbackValue));
	return (!InMontage->IsValidSectionIndex(InIndex) && InFallbackValue >= 0.0f)
		? (InFallbackValue)
		: (InMontage->GetSectionLength(InIndex) * InTimeMul);
}

float UScWAnimationsFunctionLibrary::GetMontageSectionLengthByName(const UAnimMontage* InMontage, const FName& InName, float InTimeMul, float InFallbackValue)
{
	ensureReturn(InMontage, FMath::Max(0.0f, InFallbackValue));
	return UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndex(InMontage, InMontage->GetSectionIndex(InName));
}

float UScWAnimationsFunctionLibrary::GetMontagePlayLengthFromData(const FScWCharacterMontageData& InCharacterMontageData, float InFallbackValue)
{
	return GetMontagePlayLength(InCharacterMontageData.GetRelevantTimingMontage(), InCharacterMontageData.TimeMul, InFallbackValue);
}

float UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(const FScWCharacterMontageData& InCharacterMontageData, int32 InIndex, float InFallbackValue)
{
	return GetMontageSectionLengthByIndex(InCharacterMontageData.GetRelevantTimingMontage(), InIndex, InCharacterMontageData.TimeMul, InFallbackValue);
}

float UScWAnimationsFunctionLibrary::GetMontageSectionLengthByNameFromData(const FScWCharacterMontageData& InCharacterMontageData, const FName& InName, float InFallbackValue)
{
	return GetMontageSectionLengthByName(InCharacterMontageData.GetRelevantTimingMontage(), InName, InCharacterMontageData.TimeMul, InFallbackValue);
}

float UScWAnimationsFunctionLibrary::PlayCharacterMontagesFromData(AScWCharacter* InCharacter, const FScWCharacterMontageData& InCharacterMontageData, const bool bInStopAllMontages, const bool bInPlayFirstPerson, const bool bInPlayThirdPerson, const bool bInPlayHandheld)
{
	ensureReturn(InCharacter, 0.0f);

	float PlayRateInv = 1.0f / FMath::Max(InCharacterMontageData.TimeMul, FLT_EPSILON);
	float OutMaxDuration = 0.0f;

	if (bInPlayFirstPerson)
	{
		if (UScWAnimInstance_FirstPerson* FirstPersonAnimInstance = InCharacter->GetScWFirstPersonAnimInstance())
		{
			ensure(InCharacterMontageData.FirstPersonMontage);
			OutMaxDuration = FMath::Max(FirstPersonAnimInstance->Montage_Play(InCharacterMontageData.FirstPersonMontage, PlayRateInv, EMontagePlayReturnType::Duration, 0.0f, bInStopAllMontages), 0.0f);
		}
	}
	if (bInPlayThirdPerson)
	{
		if (UScWAnimInstance_ThirdPerson* ThirdPersonAnimInstance = InCharacter->GetScWThirdPersonAnimInstance())
		{
			ensure(InCharacterMontageData.ThirdPersonMontage);
			OutMaxDuration = FMath::Max(ThirdPersonAnimInstance->Montage_Play(InCharacterMontageData.ThirdPersonMontage, PlayRateInv, EMontagePlayReturnType::Duration, 0.0f, bInStopAllMontages), 0.0f);
		}
	}
	if (bInPlayHandheld)
	{
		if (UScWAnimInstance_Handheld* HandheldAnimInstance = InCharacter->GetScWHandheldAnimInstance())
		{
			ensure(InCharacterMontageData.ActiveHandheldMontage);
			OutMaxDuration = FMath::Max(HandheldAnimInstance->Montage_Play(InCharacterMontageData.ActiveHandheldMontage, PlayRateInv, EMontagePlayReturnType::Duration, 0.0f, bInStopAllMontages), 0.0f);
		}
	}
	return OutMaxDuration;
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
