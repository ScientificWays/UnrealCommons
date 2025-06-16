// G.o.o.P Studios

#include "GAS/Tasks/ATAAT_ModifyCharacterMovement.h"

#include "Characters/ATACMC_Base.h"

UATAAT_ModifyCharacterMovement::UATAAT_ModifyCharacterMovement()
{
	bTickingTask = true;
	bSimulatedTask = true;
}

UATAAT_ModifyCharacterMovement* UATAAT_ModifyCharacterMovement::ModifyCharacterMovement(UGameplayAbility* InOwningAbility, UATACMC_Base* InTarget, FVector InMagnitude, const bool bInApplyAsForce, float InTimeOut)
{
	UATAAT_ModifyCharacterMovement* OutTaskObject = NewAbilityTask<UATAAT_ModifyCharacterMovement>(InOwningAbility);

	OutTaskObject->TargetCMC = InTarget;
	OutTaskObject->Magnitude = InMagnitude;
	OutTaskObject->bApplyAsForce = bInApplyAsForce;

	OutTaskObject->TimeOut = InTimeOut;
	return OutTaskObject;
}

//~ Begin Initialize
void UATAAT_ModifyCharacterMovement::Activate() // UGameplayTask
{
	if (TimeOut > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(TimeOutTimerHandle, this, &UATAAT_ModifyCharacterMovement::OnTimeOut, TimeOut, false);
	}
	Super::Activate();
}

void UATAAT_ModifyCharacterMovement::OnDestroy(bool bInAbilityIsEnding) // UGameplayTask
{
	GetWorld()->GetTimerManager().ClearTimer(TimeOutTimerHandle);
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Tick
void UATAAT_ModifyCharacterMovement::TickTask(float InDeltaSeconds) // UGameplayTask
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
void UATAAT_ModifyCharacterMovement::OnTimeOut()
{
	OnTimeOutExec.Broadcast();
	EndTask();
}
//~ End TimeOut
