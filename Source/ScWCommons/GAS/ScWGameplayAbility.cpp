// G.o.o.P Studios

#include "GAS/ATAGA_Base.h"

#include "Characters/ATACharacter.h"

#include "GAS/ATAASC_Base.h"

#include "Player/ATAPlayerController.h"

//~ Begin Initialize
void UATAGA_Base::SetCurrentActorInfo(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo) const // UGameplayAbility
{
	Super::SetCurrentActorInfo(InHandle, InActorInfo);

	const_cast<ThisClass*>(this)->OwnerCharacter = Cast<AATACharacter>(InActorInfo->AvatarActor);
	ensure(OwnerCharacter);

	const_cast<ThisClass*>(this)->OwnerASC = Cast<UATAASC_Base>(InActorInfo->AbilitySystemComponent);
	ensure(OwnerASC);

	if (InActorInfo->PlayerController.IsValid())
	{
		const_cast<ThisClass*>(this)->OwnerPlayerController = Cast<AATAPlayerController>(InActorInfo->PlayerController);
		ensure(OwnerPlayerController);
	}
}
//~ End Initialize

//~ Begin Events
void UATAGA_Base::ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UGameplayAbility
{
	Super::ActivateAbility(InHandle, InActorInfo, InActivationInfo, InTriggerEventData);


}

void UATAGA_Base::CancelAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateCancelAbility) // UGameplayAbility
{
	Super::CancelAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateCancelAbility);


}

void UATAGA_Base::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);
	

}
//~ End Events
