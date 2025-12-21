// Scientific Ways

#include "Gameplay/Abilities/ScWGameplayAbility_ComboSwing.h"

#include "Gameplay/ScWASC_Character.h"
#include "Gameplay/Combo/ScWComboData.h"
#include "Gameplay/Tasks/ScWAT_WaitDelay.h"
#include "Gameplay/Tasks/ScWAT_WaitComboMoveEvent.h"
#include "Gameplay/Handhelds/ScWHandheldData_Melee.h"

UScWGameplayAbility_ComboSwing::UScWGameplayAbility_ComboSwing()
{
	
}

//~ Begin Ability
void UScWGameplayAbility_ComboSwing::NativeActivateAbility_Committed(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UGameplayAbility
{
	ensureCancelAbilityReturn(OwnerASC);
	OwnerASC->AcceptQueuedComboMove();

	Super::NativeActivateAbility_Committed(InHandle, InActorInfo, InActivationInfo, InTriggerEventData);
}

void UScWGameplayAbility_ComboSwing::OnPostSwingComboWindowFinished()
{
	ensureCancelAbilityReturn(OwnerASC);
	OwnerASC->SetComboState(EComboState::Reset, true);
}

void UScWGameplayAbility_ComboSwing::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	if (bInWasCancelled)
	{
		OwnerASC->SetComboState(EComboState::Reset, true);
	}
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);
}
//~ End Ability

//~ Begin Swing
float UScWGameplayAbility_ComboSwing::BP_HandlePreSwing_Implementation() // UScWGameplayAbility_CommonSwing
{
	ensureCancelAbilityReturn(OwnerASC, Super::BP_HandlePreSwing_Implementation());
	OwnerASC->SetComboState(EComboState::NotYetReadyForMove);

	return Super::BP_HandlePreSwing_Implementation();
}

float UScWGameplayAbility_ComboSwing::BP_HandleEndSwing_Implementation()
{
	float OutPostSwingDelay = Super::BP_HandleEndSwing_Implementation();

	BP_HandleSwingEndComboWindow();

	ensureCancelAbilityReturn(OwnerMeleeData, OutPostSwingDelay);
	return FMath::Max(OwnerMeleeData->PostSwingComboTimeWindow, OutPostSwingDelay);
}

void UScWGameplayAbility_ComboSwing::BP_HandlePostSwing_Implementation()
{
	ensureCancelAbilityReturn(OwnerASC);
	OwnerASC->SetComboState(EComboState::Reset, true);

	Super::BP_HandlePostSwing_Implementation();
}

void UScWGameplayAbility_ComboSwing::BP_HandleSwingEndComboWindow_Implementation()
{
	UScWAT_WaitComboMoveEvent* WaitComboMoveQueuedTask = UScWAT_WaitComboMoveEvent::WaitComboMoveQueued(this);
	WaitComboMoveQueuedTask->OnComboMoveEvent.AddDynamic(this, &ThisClass::K2_EndAbility);
	WaitComboMoveQueuedTask->ReadyForActivation();

	ensureCancelAbilityReturn(OwnerASC);
	OwnerASC->SetComboState(EComboState::ReadyForMove);

	ensureCancelAbilityReturn(OwnerMeleeData);
	UScWAT_WaitDelay* PostSwingComboWindowTask = UScWAT_WaitDelay::WaitDelayOrFinishNextTick(this, OwnerMeleeData->PostSwingComboTimeWindow);
	PostSwingComboWindowTask->OnFinish.AddDynamic(this, &ThisClass::OnPostSwingComboWindowFinished);
	PostSwingComboWindowTask->ReadyForActivation();
}
//~ End Swing
