// Scientific Ways

#include "Gameplay/Tasks/ScWAT_WaitComboStateChanged.h"

#include "Gameplay/ScWASC_Base.h"

//~ Begin Initialize
UScWAT_WaitComboStateChanged* UScWAT_WaitComboStateChanged::WaitComboStateChanged(UGameplayAbility* InOwningAbility)
{
	UScWAT_WaitComboStateChanged* OutTaskObject = NewAbilityTask<UScWAT_WaitComboStateChanged>(InOwningAbility);
	OutTaskObject->CallbackScriptDelegate.BindUFunction(OutTaskObject, TEXT("OnComboStateChangedCallback"));
	return OutTaskObject;
}

void UScWAT_WaitComboStateChanged::Activate() // UGameplayTask
{
	if (UScWASC_Base* TargetASC = Cast<UScWASC_Base>(AbilitySystemComponent))
	{
		TargetASC->OnComboStateChangedDelegate.Add(CallbackScriptDelegate);
	}
	else
	{
		EndTask();
	}
}

void UScWAT_WaitComboStateChanged::OnDestroy(bool bInAbilityIsEnding)
{
	
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Callbacks
void UScWAT_WaitComboStateChanged::OnComboStateChangedCallback()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComboStateChanged.Broadcast();
	}
	EndTask();
}
//~ End Callbacks
