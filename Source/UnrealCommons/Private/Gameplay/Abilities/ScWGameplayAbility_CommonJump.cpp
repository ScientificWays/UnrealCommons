// Scientific Ways

#include "Gameplay/Abilities/ScWGameplayAbility_CommonJump.h"

#include "Gameplay/ScWGameplayTags.h"
#include "Gameplay/Tasks/ScWAT_WaitLanded.h"
#include "Gameplay/Characters/ScWCharacter.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

UScWGameplayAbility_CommonJump::UScWGameplayAbility_CommonJump()
{
	{
		auto Tags = GetAssetTags();
		Tags.AddTag(FScWGameplayTags::Ability_Movement_Jump);
		SetAssetTags(Tags);
	}
	CancelAbilitiesWithTag.AddTag(FScWGameplayTags::Ability_CancelBy_JumpActivate);
	BlockAbilitiesWithTag.AddTag(FScWGameplayTags::Ability_CancelBy_JumpActivate);

	ActivationOwnedTags.AddTag(FScWGameplayTags::State_Jumping);

	bStopJumpingOnAbilityEnd = true;
}

//~ Begin Ability
void UScWGameplayAbility_CommonJump::NativeActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UGameplayAbility
{
	if (!CommitAbility(InHandle, InActorInfo, InActivationInfo))
	{			
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(InHandle, InActorInfo, InActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
	ensureReturn(OwnerCharacter);
	OwnerCharacter->Jump();

	UScWAT_WaitLanded* WaitLandedTask = UScWAT_WaitLanded::WaitLanded(this, OwnerCharacter);
	WaitLandedTask->OnPlayerLanded.AddDynamic(this, &ThisClass::OnPlayerLandedCallback);
	WaitLandedTask->ReadyForActivation();

	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnAbilityInputReleaseCallback);
	WaitInputReleaseTask->ReadyForActivation();
}

void UScWGameplayAbility_CommonJump::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
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
void UScWGameplayAbility_CommonJump::OnPlayerLandedCallback(const FHitResult& InHitResult)
{
	K2_EndAbility();
}

void UScWGameplayAbility_CommonJump::OnAbilityInputReleaseCallback(float InTimeHeld)
{
	K2_EndAbility();
}
//~ End Jump
