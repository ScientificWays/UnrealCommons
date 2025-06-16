// G.o.o.P Studios

#include "GAS/Tasks/ATAAT_WaitComboStateChanged.h"

#include "GAS/ATAASC_Base.h"

UATAAT_WaitComboStateChanged::UATAAT_WaitComboStateChanged()
{
	
}

UATAAT_WaitComboStateChanged* UATAAT_WaitComboStateChanged::WaitComboStateChanged(UGameplayAbility* InOwningAbility)
{
	UATAAT_WaitComboStateChanged* OutTaskObject = NewAbilityTask<UATAAT_WaitComboStateChanged>(InOwningAbility);
	OutTaskObject->CallbackScriptDelegate.BindUFunction(OutTaskObject, TEXT("OnComboStateChangedCallback"));
	return OutTaskObject;
}

//~ Begin Initialize
void UATAAT_WaitComboStateChanged::Activate() // UGameplayTask
{
	if (UATAASC_Base* TargetASC = Cast<UATAASC_Base>(AbilitySystemComponent))
	{
		TargetASC->OnComboStateChangedDelegate.Add(CallbackScriptDelegate);
	}
	else
	{
		EndTask();
	}
}

void UATAAT_WaitComboStateChanged::OnDestroy(bool bInAbilityIsEnding)
{
	
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Callbacks
void UATAAT_WaitComboStateChanged::OnComboStateChangedCallback()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComboStateChanged.Broadcast();
	}
	EndTask();
}
//~ End Callbacks
