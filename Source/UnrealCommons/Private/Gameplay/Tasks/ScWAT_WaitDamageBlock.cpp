// Scientific Ways

#include "Gameplay/Tasks/ScWAT_WaitDamageBlock.h"

#include "Gameplay/ScWASC_Base.h"

UScWAT_WaitDamageBlock* UScWAT_WaitDamageBlock::WaitDamageBlock(UGameplayAbility* InOwningAbility, UScWASC_Base* InTargetASC, TSubclassOf<UDamageType> InDamageTypeClass, const bool bOnce)
{
	UScWAT_WaitDamageBlock* OutTaskObject = NewAbilityTask<UScWAT_WaitDamageBlock>(InOwningAbility);

	OutTaskObject->TargetASC = InTargetASC;
	OutTaskObject->bOnce = bOnce;
	OutTaskObject->DamageTypeClass = InDamageTypeClass;

	return OutTaskObject;
}

void UScWAT_WaitDamageBlock::Activate()
{
	ensure(TargetASC);
	if (TargetASC)
	{
		TargetASC->OnDamageBlocked.AddDynamic(this, &UScWAT_WaitDamageBlock::OnDamageBlockedCallback);
		Super::Activate();
	}
	else
	{
		EndTask();
	}
}

void UScWAT_WaitDamageBlock::OnDestroy(bool bInAbilityIsEnding)
{
	ensure(TargetASC);
	if (TargetASC)
	{
		TargetASC->OnDamageBlocked.RemoveDynamic(this, &UScWAT_WaitDamageBlock::OnDamageBlockedCallback);
	}
	Super::OnDestroy(bInAbilityIsEnding);
}

void UScWAT_WaitDamageBlock::OnDamageBlockedCallback(float InDamage, const FReceivedDamageData& InData)
{
	if (!DamageTypeClass || (InData.DamageType && InData.DamageType->IsA(DamageTypeClass)))
	{
		OnDamageBlocked.Broadcast();

		if (bOnce)
		{
			EndTask();
		}
	}
}
