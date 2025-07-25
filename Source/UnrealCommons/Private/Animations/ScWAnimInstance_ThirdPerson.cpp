// Scientific Ways

#include "Animations/ScWAnimInstance_ThirdPerson.h"

UScWAnimInstance_ThirdPerson::UScWAnimInstance_ThirdPerson()
{
	
}

//~ Begin Static
void UScWAnimInstance_ThirdPerson::SetCurrentStaticAnimationData(const FScWThirdPersonStaticAnimationData& InStaticAnimationData)
{
	CurrentStaticAnimationData = InStaticAnimationData;
	BP_OnCurrentStaticAnimationDataChanged();
}

void UScWAnimInstance_ThirdPerson::SetDefaultStaticAnimationData(const FScWThirdPersonStaticAnimationData& InStaticAnimationData)
{
	DefaultStaticAnimationData = InStaticAnimationData;
	BP_OnDefaultStaticAnimationDataChanged();
}
//~ End Static
