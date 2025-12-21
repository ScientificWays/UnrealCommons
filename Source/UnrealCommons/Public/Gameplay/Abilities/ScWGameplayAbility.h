// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWGameplayAbility.generated.h"

#define ensureCancelAbilityReturn(InCondition, ...) ensure(InCondition); if (!(InCondition)) { K2_CancelAbility(); return __VA_ARGS__; };

#define COMMON_WAIT_DELAY_OR_FINISH_NEXT_TICK_TASK(InVariableName, InTime, InMethodName) \
	UScWAT_WaitDelay* InVariableName = UScWAT_WaitDelay::WaitDelayOrFinishNextTick(this, InTime); \
	InVariableName->OnFinish.AddDynamic(this, &ThisClass::InMethodName); \
	InVariableName->ReadyForActivation();

/**
 * 
 */
UCLASS(Abstract, meta = (DisplayName = "[ScW] Gameplay Ability"))
class UNREALCOMMONS_API UScWGameplayAbility : public UGameplayAbility
{

	GENERATED_BODY()

public:

	UScWGameplayAbility();

//~ Begin Initialize
protected:
	virtual void SetCurrentActorInfo(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo) const override; // UGameplayAbility

	UFUNCTION(Category = "Initialize", BlueprintImplementableEvent, meta = (DisplayName = "Post SetCurrentActorInfo"))
	void BP_PostSetCurrentActorInfo();
//~ End Initialize
	
//~ Begin Owner
public:

	UPROPERTY(Category = "Owner", Transient, BlueprintReadOnly)
	TObjectPtr<class AScWCharacter> OwnerCharacter;

	UPROPERTY(Category = "Owner", Transient, BlueprintReadOnly)
	TObjectPtr<class UScWASC_Base> OwnerASC;

	UPROPERTY(Category = "Owner", Transient, BlueprintReadOnly)
	TObjectPtr<class AScWPlayerController> OwnerPlayerController;
//~ End Owner
	
//~ Begin Ability
protected:
	UE_DEPRECATED(5.6, "Please check if UScWGameplayAbility::ActivateAbility() matches base UGameplayAbility::ActivateAbility() implementation and increase allowed UE version.")
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) final; // UGameplayAbility
	virtual void NativeActivateAbility_Committed(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) {}
	virtual void CancelAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateCancelAbility) override; // UGameplayAbility
	virtual void EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) override; // UGameplayAbility
//~ End Ability
	
//~ Begin Input
public:

	UFUNCTION(Category = "Input", BlueprintCallable, meta = (KeyWords = "IsInputPressed"))
	bool IsAbilityInputPressed() const;
//~ End Input
	
//~ Begin Effects
public:

	UPROPERTY(Category = "Effects", BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> DefaultGameplayEffectsClasses;

	UPROPERTY(Category = "Effects", BlueprintReadOnly)
	bool bRemoveDefaultGameplayEffectsOnAbilityEnd;

protected:

	UFUNCTION(Category = "Effects", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Apply DefaultGameplayEffects"))
	void BP_ApplyDefaultGameplayEffects();

	UFUNCTION(Category = "Effects", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Remove DefaultGameplayEffects"))
	void BP_RemoveDefaultGameplayEffects();

	UPROPERTY(Category = "Effects", BlueprintReadOnly)
	TArray<FActiveGameplayEffectHandle> AppliedDefaultGameplayEffectsHandles;
//~ End Effects
	
//~ Begin AI
public:

	UFUNCTION(Category = "AI", BlueprintCallable, meta = (AutoCreateRefTerm = "InMessage", KeyWords = "TrySendOwnerAIMessage"))
	bool TrySendAIMessageToOwner(const FName& InMessage, bool bInAsSuccess = true);
//~ End AI
};
