// Scientific Ways

#include "Gameplay/Tasks/ScWAT_WaitComboMoveEvent.h"

#include "Gameplay/ScWASC_Base.h"

UScWAT_WaitComboMoveEvent::UScWAT_WaitComboMoveEvent()
{
	
}

UScWAT_WaitComboMoveEvent* UScWAT_WaitComboMoveEvent::WaitComboMoveCommonInit(UGameplayAbility* InOwningAbility, int32 InEventType)
{
	ThisClass* OutTaskObject = NewAbilityTask<ThisClass>(InOwningAbility);
	OutTaskObject->EventType = InEventType;
	OutTaskObject->CallbackScriptDelegate.BindUFunction(OutTaskObject, TEXT("OnComboMoveEventCallback"));
	return OutTaskObject;
}

//~ Begin Initialize
void UScWAT_WaitComboMoveEvent::Activate() // UGameplayTask
{
	if (UScWASC_Base* TargetASC = Cast<UScWASC_Base>(AbilitySystemComponent))
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

void UScWAT_WaitComboMoveEvent::OnDestroy(bool bInAbilityIsEnding)
{
	if (UScWASC_Base* TargetASC = Cast<UScWASC_Base>(AbilitySystemComponent))
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
void UScWAT_WaitComboMoveEvent::OnComboMoveEventCallback()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComboMoveEvent.Broadcast();
	}
	EndTask();
}
//~ End Callbacks
