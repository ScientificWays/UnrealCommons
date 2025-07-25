// Scientific Ways

#include "Gameplay/Tasks/ScWAT_WaitInput.h"

#include "Gameplay/ScWASC_Base.h"

//~ Begin Initialize
UScWAT_WaitInput* UScWAT_WaitInput::WaitInputPressByInputID(UGameplayAbility* InOwningAbility, EScWAbilityInputID InInputID, float InExpireTime, const bool bCheckAlreadyPressed)
{
	ThisClass* OutTaskObject = NewAbilityTask<ThisClass>(InOwningAbility);

	OutTaskObject->bWaitMulti = false;
	OutTaskObject->bWaitRelease = false;

	OutTaskObject->InputID = static_cast<int32>(InInputID);

	OutTaskObject->CurrentPressNum = 0;
	OutTaskObject->TargetPressNum = 1;
	OutTaskObject->PressMaxDelay = 0.0f;

	OutTaskObject->ExpireTime = InExpireTime;
	OutTaskObject->bCheckOnActivate = bCheckAlreadyPressed;
	OutTaskObject->bReplicateInputDirectly = InOwningAbility->bReplicateInputDirectly;

	return OutTaskObject;
}

UScWAT_WaitInput* UScWAT_WaitInput::WaitInputPressByInputID_Multi(UGameplayAbility* InOwningAbility, EScWAbilityInputID InInputID, int32 InPressNum, float InPressMaxDelay, float InExpireTime, const bool bCheckAlreadyPressed)
{
	ThisClass* OutTaskObject = NewAbilityTask<ThisClass>(InOwningAbility);

	OutTaskObject->bWaitMulti = true;
	OutTaskObject->bWaitRelease = false;

	OutTaskObject->InputID = static_cast<int32>(InInputID);

	OutTaskObject->CurrentPressNum = 0;
	OutTaskObject->TargetPressNum = InPressNum;
	OutTaskObject->PressMaxDelay = InPressMaxDelay;

	OutTaskObject->ExpireTime = InExpireTime;
	OutTaskObject->bCheckOnActivate = bCheckAlreadyPressed;
	OutTaskObject->bReplicateInputDirectly = InOwningAbility->bReplicateInputDirectly;

	return OutTaskObject;
}

UScWAT_WaitInput* UScWAT_WaitInput::WaitInputReleaseByInputID(UGameplayAbility* InOwningAbility, EScWAbilityInputID InInputID, float InExpireTime, const bool bCheckAlreadyReleased)
{
	ThisClass* OutTaskObject = NewAbilityTask<ThisClass>(InOwningAbility);

	OutTaskObject->bWaitMulti = false;
	OutTaskObject->bWaitRelease = true;

	OutTaskObject->InputID = static_cast<int32>(InInputID);

	OutTaskObject->CurrentPressNum = 0;
	OutTaskObject->TargetPressNum = 1;
	OutTaskObject->PressMaxDelay = 0.0f;

	OutTaskObject->ExpireTime = InExpireTime;
	OutTaskObject->bCheckOnActivate = bCheckAlreadyReleased;
	OutTaskObject->bReplicateInputDirectly = InOwningAbility->bReplicateInputDirectly;

	return OutTaskObject;
}

UScWAT_WaitInput* UScWAT_WaitInput::WaitInputReleaseByInputID_Multi(UGameplayAbility* InOwningAbility, EScWAbilityInputID InInputID, int32 InReleaseNum, float InReleaseMaxDelay, float InExpireTime, bool bCheckAlreadyReleased)
{
	ThisClass* OutTaskObject = NewAbilityTask<ThisClass>(InOwningAbility);

	OutTaskObject->bWaitMulti = true;
	OutTaskObject->bWaitRelease = true;

	OutTaskObject->InputID = static_cast<int32>(InInputID);

	OutTaskObject->CurrentPressNum = 0;
	OutTaskObject->TargetPressNum = InReleaseNum;
	OutTaskObject->PressMaxDelay = InReleaseMaxDelay;

	OutTaskObject->ExpireTime = InExpireTime;
	OutTaskObject->bCheckOnActivate = bCheckAlreadyReleased;
	OutTaskObject->bReplicateInputDirectly = InOwningAbility->bReplicateInputDirectly;

	return OutTaskObject;
}

void UScWAT_WaitInput::Activate() // UGameplayTask
{
	BaseASC = Cast<UScWASC_Base>(AbilitySystemComponent);

	if (BaseASC)
	{
		if (bCheckOnActivate && (bWaitRelease != BaseASC->IsInputPressed(InputID)))
		{
			OnInputTriggerCallback(InputID);
		}
		if (bWaitRelease)
		{
			BaseASC->OnInputReleasedDelegate.AddDynamic(this, &UScWAT_WaitInput::OnInputTriggerCallback);
		}
		else
		{
			BaseASC->OnInputPressedDelegate.AddDynamic(this, &UScWAT_WaitInput::OnInputTriggerCallback);
		}
		if (ExpireTime > 0.0f)
		{
			FTimerManager& WorldTimerManager = GetWorld()->GetTimerManager();
			WorldTimerManager.SetTimer(ExpireTimerHandle, this, &ThisClass::ExpireCallback, ExpireTime, false);
		}
		Super::Activate();
	}
	else
	{
		EndTask();
	}
}

void UScWAT_WaitInput::OnDestroy(bool bInAbilityIsEnding) // UGameplayTask
{
	FTimerManager& WorldTimerManager = GetWorld()->GetTimerManager();

	WorldTimerManager.ClearTimer(CallbackAddTimerHandle);
	WorldTimerManager.ClearTimer(CurrentDelayTimerHandle);
	WorldTimerManager.ClearTimer(ExpireTimerHandle);

	if (BaseASC)
	{
		if (bWaitRelease)
		{
			BaseASC->OnInputReleasedDelegate.RemoveDynamic(this, &UScWAT_WaitInput::OnInputTriggerCallback);
		}
		else
		{
			BaseASC->OnInputPressedDelegate.RemoveDynamic(this, &UScWAT_WaitInput::OnInputTriggerCallback);
		}
	}
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Task
void UScWAT_WaitInput::OnInputTriggerCallback(int32 InInputID)
{
	check(TaskState != EGameplayTaskState::Finished);

	if (InInputID != InputID)
	{
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(CurrentDelayTimerHandle);

	++CurrentPressNum;
	check(CurrentPressNum <= TargetPressNum);

	if (bReplicateInputDirectly && !BaseASC->IsOwnerActorAuthoritative())
	{
		if (bWaitRelease)
		{
			BaseASC->Server_SetInputReleasedFromWaitInputTask(InInputID);
		}
		else
		{
			BaseASC->Server_SetInputPressedFromWaitInputTask(InInputID);
		}
	}
	if (CurrentPressNum == TargetPressNum)
	{
		OnTrigger.Broadcast();
		EndTask();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(CurrentDelayTimerHandle, this, &ThisClass::PressReset, PressMaxDelay, false);
	}
}

void UScWAT_WaitInput::PressReset()
{
	CurrentPressNum = 0;
}

void UScWAT_WaitInput::ExpireCallback()
{
	OnExpire.Broadcast();
	EndTask();
}
//~ End Task
