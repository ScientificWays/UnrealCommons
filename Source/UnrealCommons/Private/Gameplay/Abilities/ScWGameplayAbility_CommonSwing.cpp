// Scientific Ways

#include "Gameplay/Abilities/ScWGameplayAbility_CommonSwing.h"

#include "AI/ScWTypes_AI.h"

#include "Animations/ScWAnimationsFunctionLibrary.h"

#include "Gameplay/ScWDamageType.h"
#include "Gameplay/ScWGameplayTags.h"
#include "Gameplay/ScWASC_Character.h"
#include "Gameplay/Tasks/ScWAT_WaitDelay.h"
#include "Gameplay/Characters/ScWCharacter.h"
#include "Gameplay/Handhelds/ScWHandheld_Melee.h"
#include "Gameplay/Handhelds/ScWHandheldData_Melee.h"
#include "Gameplay/Tasks/ScWAT_ModifyCharacterMovement.h"

UScWGameplayAbility_CommonSwing::UScWGameplayAbility_CommonSwing()
{
	{
		auto Tags = GetAssetTags();
		Tags.AddTag(FScWGameplayTags::Ability_Handheld_Swing);
		SetAssetTags(Tags);
	}
	CancelAbilitiesWithTag.AddTag(FScWGameplayTags::Ability_CancelBy_SwingActivate);
	BlockAbilitiesWithTag.AddTag(FScWGameplayTags::Ability_CancelBy_SwingActivate);

	ActivationOwnedTags.AddTag(FScWGameplayTags::State_Swinging);

	bCaptureDamageDataOnBeginSwingSequence = true;

	bLoopIfInputIsPressed = true;
}

//~ Begin Ability
void UScWGameplayAbility_CommonSwing::NativeActivateAbility_Committed(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UScWGameplayAbility
{
	ensureCancelAbilityReturn(OwnerCharacter);

	OwnerMelee = OwnerCharacter->GetHandheld<AScWHandheld_Melee>();
	ensureCancelAbilityReturn(OwnerMelee);
	
	OwnerMeleeData = OwnerMelee->GetMeleeDataAsset();
	ensureCancelAbilityReturn(OwnerMeleeData);

	BeginSwingSequence();
}

void UScWGameplayAbility_CommonSwing::BeginSwingSequence()
{
	if (bCaptureDamageDataOnBeginSwingSequence)
	{
		CapturedSwingDamage = BP_GetSwingDamage();
		CapturedSwingDamageTypeClass = BP_GetSwingDamageTypeClass();
	}
	float PreSwingDelay = BP_HandlePreSwing();

	UScWAT_WaitDelay* PreSwingWaitDelayTask = UScWAT_WaitDelay::WaitDelayOrFinishNextTick(this, PreSwingDelay);
	PreSwingWaitDelayTask->OnFinish.AddDynamic(this, &ThisClass::OnPreSwingDelayFinished);
	PreSwingWaitDelayTask->ReadyForActivation();
}

void UScWGameplayAbility_CommonSwing::OnPreSwingDelayFinished()
{
	float SwingDuration = BP_HandleBeginSwing();

	UScWAT_WaitDelay* PrePatternsWaitDelayTask = UScWAT_WaitDelay::WaitDelayOrFinishNextTick(this, SwingDuration);
	PrePatternsWaitDelayTask->OnFinish.AddDynamic(this, &ThisClass::OnSwingFinished);
	PrePatternsWaitDelayTask->ReadyForActivation();
}

void UScWGameplayAbility_CommonSwing::OnSwingFinished()
{
	float PostSwingDelay = BP_HandleEndSwing();

	if (bLoopIfInputIsPressed && IsAbilityInputPressed())
	{
		BeginSwingSequence();
	}
	else
	{
		UScWAT_WaitDelay* PrePatternsWaitDelayTask = UScWAT_WaitDelay::WaitDelayOrFinishNextTick(this, PostSwingDelay);
		PrePatternsWaitDelayTask->OnFinish.AddDynamic(this, &ThisClass::OnPostSwingDelayFinished);
		PrePatternsWaitDelayTask->ReadyForActivation();
	}
}

void UScWGameplayAbility_CommonSwing::OnPostSwingDelayFinished()
{
	BP_HandlePostSwing();
}

void UScWGameplayAbility_CommonSwing::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);

	TrySendAIMessageToOwner(FScWAIMessage::SwingEnded, !bInWasCancelled);
	
	if (CurrentSwingEffectHandle.IsValid())
	{
		BP_RemoveSwingOwnerEffect();
	}
	ensureReturn(OwnerMelee);
	if (OwnerMelee->GetCurrentSwingPhase() != EScWSwingPhase::None)
	{
		OwnerMelee->BP_EndSwing(bInWasCancelled);
	}
}
//~ End Ability

//~ Begin Swing
float UScWGameplayAbility_CommonSwing::BP_HandlePreSwing_Implementation()
{
	ensureCancelAbilityReturn(OwnerMelee, 0.0f);
	float OutPreSwingDelay = OwnerMelee->BP_PreSwing();
	ensureCancelAbilityReturn(OutPreSwingDelay >= 0.0f, 0.0f);

	ensureCancelAbilityReturn(OwnerMeleeData, 0.0f);
	if (OwnerMeleeData->SwingOwnerEffect && !OwnerMeleeData->bSwingOwnerEffectOnlyDuringSwing)
	{
		BP_ApplySwingOwnerEffect();
	}
	return OutPreSwingDelay;
}

float UScWGameplayAbility_CommonSwing::BP_HandleBeginSwing_Implementation()
{
	ensureCancelAbilityReturn(OwnerMelee, 0.0f);

	float OutSwingDuration = 0.0f;

	if (bCaptureDamageDataOnBeginSwingSequence)
	{
		OutSwingDuration = OwnerMelee->BP_BeginSwing(CapturedSwingDamage, CapturedSwingDamageTypeClass);
	}
	else
	{
		OutSwingDuration = OwnerMelee->BP_BeginSwing(BP_GetSwingDamage(), BP_GetSwingDamageTypeClass());
	}
	ensureCancelAbilityReturn(OutSwingDuration >= 0.0f, 0.0f);
	ensureCancelAbilityReturn(OwnerMeleeData, 0.0f);
	ensureCancelAbilityReturn(OwnerCharacter, 0.0f);

	if (OwnerMeleeData->SwingOwnerEffect && OwnerMeleeData->bSwingOwnerEffectOnlyDuringSwing)
	{
		BP_ApplySwingOwnerEffect();
	}
	if (OwnerMeleeData->bPushOwnerDuringSwing)
	{
		FVector PushMagnitude = OwnerCharacter->GetActorRotation().RotateVector(OwnerMeleeData->SwingPushVector);

		UScWAT_ModifyCharacterMovement* PushOwnerTask = UScWAT_ModifyCharacterMovement::ModifyCharacterMovement(this, OwnerCharacter->GetScWCharacterMovement(), PushMagnitude, false, OwnerMeleeData->SwingPushDuration);
		PushOwnerTask->ReadyForActivation();
	}
	return OutSwingDuration;
}

float UScWGameplayAbility_CommonSwing::BP_HandleEndSwing_Implementation()
{
	ensureCancelAbilityReturn(OwnerMelee, 0.0f);
	float OutPostSwingDelay = OwnerMelee->BP_EndSwing(false);
	ensureCancelAbilityReturn(OutPostSwingDelay >= 0.0f, 0.0f);

	ensureCancelAbilityReturn(OwnerMeleeData, 0.0f);
	if (CurrentSwingEffectHandle.IsValid() && OwnerMeleeData->bSwingOwnerEffectOnlyDuringSwing)
	{
		BP_RemoveSwingOwnerEffect();
	}
	return OutPostSwingDelay;
}

void UScWGameplayAbility_CommonSwing::BP_HandlePostSwing_Implementation()
{
	if (CurrentSwingEffectHandle.IsValid() && !OwnerMeleeData->bSwingOwnerEffectOnlyDuringSwing)
	{
		BP_RemoveSwingOwnerEffect();
	}
	K2_EndAbility();
}

float UScWGameplayAbility_CommonSwing::BP_GetSwingDamage_Implementation() const
{
	ensureReturn(OwnerMelee, 0.0f);
	return OwnerMelee->BP_GetSwingDamage();
}

TSubclassOf<UScWDamageType> UScWGameplayAbility_CommonSwing::BP_GetSwingDamageTypeClass_Implementation() const
{
	ensureReturn(OwnerMelee, UScWDamageType::StaticClass());
	return OwnerMelee->BP_GetSwingDamageTypeClass();
}

void UScWGameplayAbility_CommonSwing::BP_ApplySwingOwnerEffect_Implementation()
{
	ensureCancelAbilityReturn(OwnerMeleeData);
	ensureCancelAbilityReturn(OwnerMeleeData->SwingOwnerEffect);

	if (CurrentSwingEffectHandle.IsValid())
	{
		BP_RemoveSwingOwnerEffect();
	}
	ensureCancelAbilityReturn(OwnerASC);
	CurrentSwingEffectHandle = OwnerASC->TryApplyGameplayEffectByClass(OwnerMeleeData->SwingOwnerEffect);
}

void UScWGameplayAbility_CommonSwing::BP_RemoveSwingOwnerEffect_Implementation()
{
	ensureCancelAbilityReturn(OwnerASC);
	ensureCancelAbilityReturn(CurrentSwingEffectHandle.IsValid());
	OwnerASC->RemoveActiveGameplayEffect(CurrentSwingEffectHandle);

	CurrentSwingEffectHandle.Invalidate();
}
//~ End Swing
