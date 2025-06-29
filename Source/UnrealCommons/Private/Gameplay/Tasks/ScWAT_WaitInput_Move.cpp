// Scientific Ways

#include "Gameplay/Tasks/ScWAT_WaitInput_Move.h"

#include "Gameplay/Characters/ScWCharacter.h"

UScWAT_WaitInput_Move* UScWAT_WaitInput_Move::WaitInput_Move(UGameplayAbility* InOwningAbility, AScWCharacter* InTargetCharacter, bool bTriggerOnForward, bool bTriggerOnBackward, bool bTriggerOnRight, bool bTriggerOnLeft, bool bOnce)
{
	ThisClass* OutTaskObject = NewAbilityTask<ThisClass>(InOwningAbility);

	OutTaskObject->TargetCharacter = InTargetCharacter;

	OutTaskObject->bTriggerOnForward = bTriggerOnForward;
	OutTaskObject->bTriggerOnBackward = bTriggerOnBackward;
	OutTaskObject->bTriggerOnRight = bTriggerOnRight;
	OutTaskObject->bTriggerOnLeft = bTriggerOnLeft;

	OutTaskObject->bOnce = bOnce;

	return OutTaskObject;
}

//~ Begin Initialize
void UScWAT_WaitInput_Move::Activate() // UGameplayTask
{
	ensure(TargetCharacter);
	if (TargetCharacter)
	{
		InputTriggeredDelegate.BindUFunction(this, TEXT("OnInputTriggeredCallback"));

		if (bTriggerOnForward)
		{
			TargetCharacter->OnForwardInputTriggeredDelegate.Add(InputTriggeredDelegate);
		}
		if (bTriggerOnBackward)
		{
			TargetCharacter->OnBackwardInputTriggeredDelegate.Add(InputTriggeredDelegate);
		}
		if (bTriggerOnRight)
		{
			TargetCharacter->OnRightInputTriggeredDelegate.Add(InputTriggeredDelegate);
		}
		if (bTriggerOnLeft)
		{
			TargetCharacter->OnLeftInputTriggeredDelegate.Add(InputTriggeredDelegate);
		}
		Super::Activate();
	}
	else
	{
		EndTask();
	}
}

void UScWAT_WaitInput_Move::OnDestroy(bool bInAbilityIsEnding) // UGameplayTask
{
	ensure(TargetCharacter);
	if (TargetCharacter)
	{
		if (bTriggerOnForward)
		{
			TargetCharacter->OnForwardInputTriggeredDelegate.Remove(InputTriggeredDelegate);
		}
		if (bTriggerOnBackward)
		{
			TargetCharacter->OnBackwardInputTriggeredDelegate.Remove(InputTriggeredDelegate);
		}
		if (bTriggerOnRight)
		{
			TargetCharacter->OnRightInputTriggeredDelegate.Remove(InputTriggeredDelegate);
		}
		if (bTriggerOnLeft)
		{
			TargetCharacter->OnLeftInputTriggeredDelegate.Remove(InputTriggeredDelegate);
		}
	}
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Task
void UScWAT_WaitInput_Move::OnInputTriggeredCallback()
{
	OnInputTriggered.Broadcast();

	if (bOnce)
	{
		EndTask();
	}
}
//~ End Task
