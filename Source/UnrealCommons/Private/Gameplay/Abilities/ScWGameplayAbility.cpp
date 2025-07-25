// Scientific Ways

#include "Gameplay/Abilities/ScWGameplayAbility.h"

#include "AI/ScWTypes_AI.h"
#include "AI/ScWAIFunctionLibrary.h"

#include "Framework/ScWPlayerController.h"

#include "Gameplay/ScWASC_Base.h"
#include "Gameplay/ScWGameplayTags.h"
#include "Gameplay/Characters/ScWCharacter.h"

#include "AbilitySystemLog.h"

UScWGameplayAbility::UScWGameplayAbility()
{
	{
		auto Tags = GetAssetTags();
		Tags.AddTag(FScWGameplayTags::Ability_CancelBy_Stunned);
		SetAssetTags(Tags);
	}
	ActivationBlockedTags.AddTag(FScWGameplayTags::State_Stunned);
	ActivationBlockedTags.AddTag(FScWGameplayTags::State_Dead);

	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnlyTermination;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bServerRespectsRemoteAbilityCancellation = false;
}

//~ Begin Initialize
void UScWGameplayAbility::SetCurrentActorInfo(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo) const // UGameplayAbility
{
	Super::SetCurrentActorInfo(InHandle, InActorInfo);

	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::InstancedPerActor ||
		InstancingPolicy == EGameplayAbilityInstancingPolicy::InstancedPerExecution)
	{
		const_cast<ThisClass*>(this)->OwnerCharacter = Cast<AScWCharacter>(InActorInfo->AvatarActor);
		ensure(OwnerCharacter);

		const_cast<ThisClass*>(this)->OwnerASC = Cast<UScWASC_Base>(InActorInfo->AbilitySystemComponent);
		ensure(OwnerASC);

		if (InActorInfo->PlayerController.IsValid())
		{
			const_cast<ThisClass*>(this)->OwnerPlayerController = Cast<AScWPlayerController>(InActorInfo->PlayerController);
			ensure(OwnerPlayerController);
		}
	}
}
//~ End Initialize

//~ Begin Ability
void UScWGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UGameplayAbility
{
	// Based on UGameplayAbility::ActivateAbility() implementation in UE 5.6.0
	if (InTriggerEventData && bHasBlueprintActivateFromEvent)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbilityFromEvent(*InTriggerEventData);
	}
	else if (bHasBlueprintActivate)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbility();
	}
	else if (bHasBlueprintActivateFromEvent)
	{
		UE_LOG(LogAbilitySystem, Warning, TEXT("Ability %s expects event data but none is being supplied. Use 'Activate Ability' instead of 'Activate Ability From Event' in the Blueprint."), *GetName());
		constexpr bool bReplicateEndAbility = false;
		constexpr bool bWasCancelled = true;
		EndAbility(InHandle, InActorInfo, InActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
	else
	{
		if (CommitAbility(InHandle, InActorInfo, InActivationInfo))
		{
			BP_ApplyDefaultGameplayEffects();
			NativeActivateAbility_Commited(InHandle, InActorInfo, InActivationInfo, InTriggerEventData);
		}
		else
		{
			constexpr bool bReplicateEndAbility = true;
			constexpr bool bWasCancelled = true;
			EndAbility(InHandle, InActorInfo, InActivationInfo, bReplicateEndAbility, bWasCancelled);
		}
	}
}

void UScWGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateCancelAbility) // UGameplayAbility
{
	Super::CancelAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateCancelAbility);

	TrySendAIMessageToOwner(FScWAIMessage::AbilityCancelled, true);
}

void UScWGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);

	if (bRemoveDefaultGameplayEffectsOnAbilityEnd)
	{
		BP_RemoveDefaultGameplayEffects();
	}
	TrySendAIMessageToOwner(FScWAIMessage::AbilityEnded, !bInWasCancelled);
}
//~ End Ability

//~ Begin Input
bool UScWGameplayAbility::IsAbilityInputPressed() const
{
	FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	ensureReturn(AbilitySpec, false);
	return AbilitySpec->InputPressed;
}
//~ End Input

//~ Begin Effects
void UScWGameplayAbility::BP_ApplyDefaultGameplayEffects_Implementation()
{
	ensureReturn(OwnerASC);
	ensure(AppliedDefaultGameplayEffectsHandles.IsEmpty());

	for (TSubclassOf<UGameplayEffect> SampleEffectClass : DefaultGameplayEffectsClasses)
	{
		auto SampleEffectHandle = OwnerASC->TryApplyGameplayEffectByClass(SampleEffectClass);
		AppliedDefaultGameplayEffectsHandles.Add(SampleEffectHandle);
	}
}

void UScWGameplayAbility::BP_RemoveDefaultGameplayEffects_Implementation()
{
	ensureReturn(OwnerASC);

	for (const auto& SampleEffectHandle : AppliedDefaultGameplayEffectsHandles)
	{
		OwnerASC->RemoveActiveGameplayEffect(SampleEffectHandle);
	}
	AppliedDefaultGameplayEffectsHandles.Empty();
}
//~ End Effects

	
//~ Begin AI
bool UScWGameplayAbility::TrySendAIMessageToOwner(const FName& InMessage, bool bInAsSuccess)
{
	ensureReturn(CurrentActorInfo, false);

	UBrainComponent* MessageTarget = UScWAIFunctionLibrary::TryGetActorBrainComponent(CurrentActorInfo->OwnerActor.Get());
	if (!MessageTarget)
	{
		MessageTarget = UScWAIFunctionLibrary::TryGetActorBrainComponent(CurrentActorInfo->AvatarActor.Get());
	}
	if (MessageTarget)
	{
		FAIMessage::Send(MessageTarget, FAIMessage(InMessage, this, bInAsSuccess));
		return true;
	}
	return false;
}
//~ End AI
