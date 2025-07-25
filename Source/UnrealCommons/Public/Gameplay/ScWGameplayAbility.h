// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Gameplay Ability"))
class UNREALCOMMONS_API UScWGameplayAbility : public UGameplayAbility
{

	GENERATED_BODY()

public:

	UScWGameplayAbility();

//~ Begin Initialize
protected:
	virtual void SetCurrentActorInfo(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo) const override; // UGameplayAbility
//~ End Initialize
	
//~ Begin Owner
public:
	
	UPROPERTY(Category = "Owner", BlueprintReadOnly)
	TObjectPtr<class AScWCharacter> OwnerCharacter;

	UPROPERTY(Category = "Owner", BlueprintReadOnly)
	TObjectPtr<class UScWASC_Base> OwnerASC;
	
	UPROPERTY(Category = "Owner", BlueprintReadOnly)
	TObjectPtr<class AScWPlayerController> OwnerPlayerController;
//~ End Owner
	
//~ Begin Ability
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) override; // UGameplayAbility
	virtual void CancelAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateCancelAbility) override; // UGameplayAbility
	virtual void EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) override; // UGameplayAbility
//~ End Ability
	
//~ Begin Input
public:

	UFUNCTION(Category = "Input", BlueprintCallable, meta = (KeyWords = "IsInputPressed"))
	bool IsAbilityInputPressed() const;
//~ End Input
};
