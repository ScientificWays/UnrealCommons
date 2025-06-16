// G.o.o.P Studios

#include "GAS/Tasks/ATAAT_WaitComboMoveEvent.h"

#include "GAS/ATAASC_Base.h"

UATAAT_WaitComboMoveEvent::UATAAT_WaitComboMoveEvent()
{
	
}

UATAAT_WaitComboMoveEvent* UATAAT_WaitComboMoveEvent::WaitComboMoveCommonInit(UGameplayAbility* InOwningAbility, int32 InEventType)
{
	ThisClass* OutTaskObject = NewAbilityTask<ThisClass>(InOwningAbility);
	OutTaskObject->EventType = InEventType;
	OutTaskObject->CallbackScriptDelegate.BindUFunction(OutTaskObject, TEXT("OnComboMoveEventCallback"));
	return OutTaskObject;
}

//~ Begin Initialize
void UATAAT_WaitComboMoveEvent::Activate() // UGameplayTask
{
	if (UATAASC_Base* TargetASC = Cast<UATAASC_Base>(AbilitySystemComponent))
	{
		switch (EventType)
		{
			case EVENT_TYPE_QUEUED: TargetASC->OnComboMoveQueuedDelegate.Add(CallbackScriptDelegate); break;
			case EVENT_TYPE_QUEUED_ACCEPTED: TargetASC->OnQueuedComboMoveAcceptedDelegate.Add(CallbackScriptDelegate); break;
			case EVENT_TYPE_QUEUED_DENIED: TargetASC->OnQueuedComboMoveDeniedDelegate.Add(CallbackScriptDelegate); break;
			case EVENT_TYPE_ADDED: TargetASC->OnComboMoveAddedDelegate.Add(CallbackScriptDelegate); break;
		}
	}
	else
	{
		EndTask();
	}
}

void UATAAT_WaitComboMoveEvent::OnDestroy(bool bInAbilityIsEnding)
{
	if (UATAASC_Base* TargetASC = Cast<UATAASC_Base>(AbilitySystemComponent))
	{
		switch (EventType)
		{
			case EVENT_TYPE_QUEUED: TargetASC->OnComboMoveQueuedDelegate.Remove(CallbackScriptDelegate); break;
			case EVENT_TYPE_QUEUED_ACCEPTED: TargetASC->OnQueuedComboMoveAcceptedDelegate.Remove(CallbackScriptDelegate); break;
			case EVENT_TYPE_QUEUED_DENIED: TargetASC->OnQueuedComboMoveDeniedDelegate.Remove(CallbackScriptDelegate); break;
			case EVENT_TYPE_ADDED: TargetASC->OnComboMoveAddedDelegate.Remove(CallbackScriptDelegate); break;
		}
	}
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Callbacks
void UATAAT_WaitComboMoveEvent::OnComboMoveEventCallback()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComboMoveEvent.Broadcast();
	}
	EndTask();
}
//~ End Callbacks
