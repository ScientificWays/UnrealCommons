// Scientific Ways

#include "Gameplay/ScWGameplayAbility.h"

#include "Gameplay/Characters/ScWCharacter.h"

#include "Framework/ScWPlayerController.h"

#include "Gameplay/ScWASC_Base.h"
#include "Gameplay/ScWGameplayTags.h"

UScWGameplayAbility::UScWGameplayAbility()
{
	SetAssetTags(FGameplayTagContainer(FScWGameplayTags::Ability_CancelBy_Stunned));

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
//~ End Initialize

//~ Begin Events
void UScWGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UGameplayAbility
{
	Super::ActivateAbility(InHandle, InActorInfo, InActivationInfo, InTriggerEventData);


}

void UScWGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateCancelAbility) // UGameplayAbility
{
	Super::CancelAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateCancelAbility);


}

void UScWGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);
	

}
//~ End Events
