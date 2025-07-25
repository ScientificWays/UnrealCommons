// Scientific Ways

#include "Animations/ScWAnimInstance_Handheld.h"

UScWAnimInstance_Handheld::UScWAnimInstance_Handheld()
{

}

//~ Begin Static
void UScWAnimInstance_Handheld::SetCurrentStaticAnimationData(const FScWHandheldStaticAnimationData& InStaticAnimationData)
{
	CurrentStaticAnimationData = InStaticAnimationData;
	BP_OnCurrentStaticAnimationDataChanged();
}
//~ End Static
