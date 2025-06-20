// Scientific Ways

#include "Gameplay/ScWGameplayAbility.h"

#include "Characters/ScWCharacter.h"
#include "Framework/ScWPlayerController.h"

#include "Gameplay/ScWASC_Base.h"

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
