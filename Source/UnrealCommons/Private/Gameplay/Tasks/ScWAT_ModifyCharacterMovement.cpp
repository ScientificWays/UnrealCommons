// Scientific Ways

#include "Gameplay/Tasks/ScWAT_ModifyCharacterMovement.h"

#include "Gameplay/Characters/ScWCharacterMovement.h"

UScWAT_ModifyCharacterMovement::UScWAT_ModifyCharacterMovement()
{
	bTickingTask = true;
	bSimulatedTask = true;
}

UScWAT_ModifyCharacterMovement* UScWAT_ModifyCharacterMovement::ModifyCharacterMovement(UGameplayAbility* InOwningAbility, UScWCharacterMovement* InTarget, FVector InMagnitude, const bool bInApplyAsForce, float InTimeOut)
{
	UScWAT_ModifyCharacterMovement* OutTaskObject = NewAbilityTask<UScWAT_ModifyCharacterMovement>(InOwningAbility);

	OutTaskObject->TargetCMC = InTarget;
	OutTaskObject->Magnitude = InMagnitude;
	OutTaskObject->bApplyAsForce = bInApplyAsForce;

	OutTaskObject->TimeOut = InTimeOut;
	return OutTaskObject;
}

//~ Begin Initialize
void UScWAT_ModifyCharacterMovement::Activate() // UGameplayTask
{
	if (TimeOut > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(TimeOutTimerHandle, this, &UScWAT_ModifyCharacterMovement::OnTimeOut, TimeOut, false);
	}
	Super::Activate();
}

void UScWAT_ModifyCharacterMovement::OnDestroy(bool bInAbilityIsEnding) // UGameplayTask
{
	GetWorld()->GetTimerManager().ClearTimer(TimeOutTimerHandle);
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Tick
void UScWAT_ModifyCharacterMovement::TickTask(float InDeltaSeconds) // UGameplayTask
{
	if (!TargetCMC)
	{
		EndTask();
		return;
	}
	if (bApplyAsForce)
	{
		TargetCMC->AddForce(Magnitude);
	}
	else
	{
		TargetCMC->Velocity = Magnitude;
	}
}
//~ End Tick

//~ Begin TimeOut
void UScWAT_ModifyCharacterMovement::OnTimeOut()
{
	OnTimeOutExec.Broadcast();
	EndTask();
}
//~ End TimeOut
