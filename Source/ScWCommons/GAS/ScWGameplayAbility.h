// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ATAGA_Base.generated.h"

/**
 * 
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UATAGA_Base : public UGameplayAbility
{

	GENERATED_BODY()
	
//~ Begin Initialize
protected:
	virtual void SetCurrentActorInfo(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo) const override; // UGameplayAbility
//~ End Initialize
	
//~ Begin Events
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) override; // UGameplayAbility
	virtual void CancelAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateCancelAbility) override; // UGameplayAbility
	virtual void EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) override; // UGameplayAbility
//~ End Events
	

//~ Begin Owner
public:
	
	UPROPERTY(Category = "Owner", BlueprintReadOnly)
	TObjectPtr<class AATACharacter> OwnerCharacter;

	UPROPERTY(Category = "Owner", BlueprintReadOnly)
	TObjectPtr<class UATAASC_Base> OwnerASC;
	
	UPROPERTY(Category = "Owner", BlueprintReadOnly)
	TObjectPtr<class AATAPlayerController> OwnerPlayerController;
//~ End Owner
};
