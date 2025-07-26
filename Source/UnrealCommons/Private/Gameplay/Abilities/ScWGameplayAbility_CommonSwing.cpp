// Scientific Ways

#include "Gameplay/Abilities/ScWGameplayAbility_CommonSwing.h"

#include "Gameplay/ScWGameplayTags.h"
#include "Gameplay/Characters/ScWCharacter.h"

UScWGameplayAbility_CommonSwing::UScWGameplayAbility_CommonSwing()
{
	{
		auto Tags = GetAssetTags();
		Tags.AddTag(FScWGameplayTags::Ability_Handheld_Swing);
		SetAssetTags(Tags);
	}
	CancelAbilitiesWithTag.AddTag(FScWGameplayTags::Ability_CancelBy_SwingActivate);
	ActivationOwnedTags.AddTag(FScWGameplayTags::State_Swinging);
}

//~ Begin Ability
void UScWGameplayAbility_CommonSwing::NativeActivateAbility_Commited(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UGameplayAbility
{
	ensureReturn(OwnerCharacter);
	OwnerCharacter->Jump();

	UScWAT_WaitLanded* WaitLandedTask = UScWAT_WaitLanded::WaitLanded(this, OwnerCharacter);
	WaitLandedTask->OnPlayerLanded.AddDynamic(this, &ThisClass::OnPlayerLandedCallback);
	WaitLandedTask->ReadyForActivation();

	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnAbilityInputReleaseCallback);
	WaitInputReleaseTask->ReadyForActivation();
}

void UScWGameplayAbility_CommonSwing::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	if (bStopJumpingOnAbilityEnd)
	{
		ensureReturn(OwnerCharacter);
		OwnerCharacter->StopJumping();
	}
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);
}
//~ End Ability

//~ Begin Jump
void UScWGameplayAbility_CommonSwing::OnPlayerLandedCallback(const FHitResult& InHitResult)
{
	K2_EndAbility();
}

void UScWGameplayAbility_CommonSwing::OnAbilityInputReleaseCallback(float InTimeHeld)
{
	K2_EndAbility();
}
//~ End Jump
