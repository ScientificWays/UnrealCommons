// Scientific Ways

#include "Animations/ScWTypes_Animations.h"

const FScWHandheldStaticAnimationData FScWHandheldStaticAnimationData::None = FScWHandheldStaticAnimationData();
const FScWFirstPersonStaticAnimationData FScWFirstPersonStaticAnimationData::None = FScWFirstPersonStaticAnimationData();
const FScWThirdPersonStaticAnimationData FScWThirdPersonStaticAnimationData::None = FScWThirdPersonStaticAnimationData();

UAnimMontage* FScWCharacterMontageData::GetRelevantTimingMontage() const
{
	switch (TimingMontageIndex)
	{
		case 0:
		{
			return FirstPersonMontage;
		}
		case 1:
		{
			return FirstPersonActiveHandheldMontage;
		}
		case 2:
		{
			return ThirdPersonMontage;
		}
		case 3:
		{
			return ThirdPersonActiveHandheldMontage;
		}
		default:
		{
			ensureReturn(false, nullptr);
		}
	}
}
