// Scientific Ways

#include "Animations/ScWTypes_Animations.h"

const FScWFirstPersonStaticAnimationData FScWFirstPersonStaticAnimationData::None = FScWFirstPersonStaticAnimationData();
const FScWThirdPersonStaticAnimationData FScWThirdPersonStaticAnimationData::None = FScWThirdPersonStaticAnimationData();
const FScWHandheldStaticAnimationData FScWHandheldStaticAnimationData::None = FScWHandheldStaticAnimationData();

UAnimMontage* FScWCharacterMontageData::GetRelevantTimingMontage() const
{
	switch (TimingMontageType)
	{
		case EScWTimingMontageType::FirstPerson:
		{
			return FirstPersonMontage;
		}
		case EScWTimingMontageType::ThirdPerson:
		{
			return ThirdPersonMontage;
		}
		case EScWTimingMontageType::ActiveHandheld:
		{
			return ActiveHandheldMontage;
		}
	}
	ensureReturn(false, nullptr);
}
