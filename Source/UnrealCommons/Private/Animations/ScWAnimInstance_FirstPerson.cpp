// Scientific Ways

#include "Animations/ScWAnimInstance_FirstPerson.h"

UScWAnimInstance_FirstPerson::UScWAnimInstance_FirstPerson()
{
	
}

//~ Begin Static
void UScWAnimInstance_FirstPerson::SetCurrentStaticAnimationData(const FScWFirstPersonStaticAnimationData& InStaticAnimationData)
{
	CurrentStaticAnimationData = InStaticAnimationData;
	BP_OnCurrentStaticAnimationDataChanged();
}

void UScWAnimInstance_FirstPerson::SetDefaultStaticAnimationData(const FScWFirstPersonStaticAnimationData& InStaticAnimationData)
{
	DefaultStaticAnimationData = InStaticAnimationData;
	BP_OnDefaultStaticAnimationDataChanged();
}
//~ End Static
