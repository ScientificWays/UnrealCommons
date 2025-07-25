// Scientific Ways

#include "Animations/ScWAnimInstance_Base.h"

#include "Gameplay/Characters/ScWCharacter.h"

UScWAnimInstance_Base::UScWAnimInstance_Base()
{
	bUseHolsterStaticAnimationData = false;

	CurrentHolsterStaticAnimationDataBlendTime = 0.1f;

	bUseAlternativeBasePose = false;

	CurrentAlternativeBasePoseBlendTime = 0.1f;

	bEnableIdleAnimation = true;
}

//~ Begin Editor
#if WITH_EDITOR
void UScWAnimInstance_Base::PostEditChangeProperty(FPropertyChangedEvent& InPropertyChangedEvent) // UObject
{
	Super::PostEditChangeProperty(InPropertyChangedEvent);

	if (InPropertyChangedEvent.Property)
	{
		const FName& PropertyName = InPropertyChangedEvent.Property->GetFName();

		if (PropertyName == FName(TEXT("DefaultStaticAnimationData")))
		{
			ResetToDefaultStaticAnimationData();
		}
	}
}
#endif // WITH_EDITOR
//~ End Editor

//~ Begin Initialize
void UScWAnimInstance_Base::NativeInitializeAnimation() // UAnimInstance
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<AScWCharacter>(GetOwningActor());
}
//~ End Initialize

//~ Begin Static
void UScWAnimInstance_Base::SetUseHolsterStaticAnimationData(const bool bInUseHolster, const float InBlendTime)
{
	bUseHolsterStaticAnimationData = bInUseHolster;
	CurrentHolsterStaticAnimationDataBlendTime = InBlendTime;

	BP_OnUseHolsterStaticAnimationDataChanged();
}

void UScWAnimInstance_Base::SetUseAlternativeBasePose(const bool bInUseAlternative, const float InBlendTime)
{
	bUseAlternativeBasePose = bInUseAlternative;
	CurrentAlternativeBasePoseBlendTime = InBlendTime;

	BP_OnUseAlternativeBasePoseChanged();
}

void UScWAnimInstance_Base::SetEnableIdleAnimation(const bool bInEnable)
{
	bEnableIdleAnimation = bInEnable;

	BP_OnEnableIdleAnimationChanged();
}
//~ End Static
