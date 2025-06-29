// Scientific Ways

#include "Gameplay/Tasks/ScWAT_WaitDied.h"

#include "Gameplay/ScWASC_Base.h"

//~ Begin Initialize
UScWAT_WaitDied* UScWAT_WaitDied::WaitDied(UGameplayAbility* InOwningAbility, UScWASC_Base* InTargetASC)
{
	UScWAT_WaitDied* OutTaskObject = NewAbilityTask<UScWAT_WaitDied>(InOwningAbility);
	OutTaskObject->TargetASC = InTargetASC;
	return OutTaskObject;
}

void UScWAT_WaitDied::Activate()
{
	ensure(TargetASC);
	if (TargetASC)
	{
		OnDiedScriptDelegate.BindUFunction(this, TEXT("OnDiedCallback"));
		TargetASC->OnDied.Add(OnDiedScriptDelegate);
		Super::Activate();
	}
	else
	{
		EndTask();
	}
}

void UScWAT_WaitDied::OnDestroy(bool bInAbilityIsEnding)
{
	ensure(TargetASC);
	if (TargetASC)
	{
		TargetASC->OnDied.Remove(OnDiedScriptDelegate);
	}
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Death
void UScWAT_WaitDied::OnDiedCallback()
{
	OnDied.Broadcast();
	EndTask();
}
//~ End Death
