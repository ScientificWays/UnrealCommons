// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/ScWTypes_Gameplay.h"
#include "Gameplay/Abilities/ScWGameplayAbility.h"

#include "ScWGameplayAbility_CommonInteract.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Common Swing Gameplay Ability"))
class UNREALCOMMONS_API UScWGameplayAbility_CommonInteract : public UScWGameplayAbility
{

	GENERATED_BODY()

public:

	UScWGameplayAbility_CommonInteract();

//~ Begin Ability
protected:
	virtual void NativeActivateAbility_Committed(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) override; // UGameplayAbility
	virtual void BeginInteractSequence();

	UFUNCTION()
	void OnInteractCooldownFinished();

	UFUNCTION()
	void OnCanInteractCheckCooldownFinished();

	virtual void EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) override; // UGameplayAbility
//~ End Ability

//~ Begin Owner
protected:

	UPROPERTY(Category = "Owner", Transient, BlueprintReadOnly)
	TObjectPtr<class UScWInteractComponent> OwnerInteractComponent;
//~ End Owner

//~ Begin Interact
protected:

	UFUNCTION(Category = "Interact", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Try StartInteract", ReturnDisplayName = "bOutStarted"))
	bool BP_TryStartInteract();

	UFUNCTION(Category = "Interact", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Try FinishInteract", ReturnDisplayName = "bOutFinished"))
	bool BP_TryFinishInteract(const bool bInWasCancelled);

	UFUNCTION(Category = "Interact", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Try FinishInteract", ReturnDisplayName = "bOutFinished"))
	bool BP_ShouldFinishInteract(const bool bInWasCancelled);

	UFUNCTION(Category = "Interact", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetInteractCooldown"))
	void BP_HandleFinishInteract(const bool bInWasCancelled);

	UFUNCTION(Category = "Interact", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle PostInteract"))
	void BP_HandlePostInteract();

	UFUNCTION(Category = "Interact", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetInteractCooldown"))
	float BP_GetInteractCooldown() const;

	UPROPERTY(Category = "Interact", EditAnywhere, BlueprintReadWrite)
	float CanInteractCheckCooldown;

	UPROPERTY(Category = "Interact", EditAnywhere, BlueprintReadWrite)
	bool bLoopIfInputIsPressed;
//~ End Interact

//~ Begin Trace
protected:

	UPROPERTY(Category = "Trace", EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETraceTypeQuery> InteractTraceTypeQuery;

	UPROPERTY(Category = "Trace | Debug", EditAnywhere, BlueprintReadWrite)
	FScWTraceDebugData InteractTraceDebugData;
//~ End Trace
};
