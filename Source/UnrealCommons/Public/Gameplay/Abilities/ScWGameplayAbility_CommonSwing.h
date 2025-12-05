// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Abilities/ScWGameplayAbility.h"

#include "ScWGameplayAbility_CommonSwing.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Common Swing Gameplay Ability"))
class UNREALCOMMONS_API UScWGameplayAbility_CommonSwing : public UScWGameplayAbility
{

	GENERATED_BODY()

public:

	UScWGameplayAbility_CommonSwing();

//~ Begin Ability
protected:
	virtual void NativeActivateAbility_Commited(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) override; // UScWGameplayAbility
	virtual void BeginSwingSequence();

	UFUNCTION()
	void OnPreSwingDelayFinished();

	UFUNCTION()
	void OnSwingFinished();

	UFUNCTION()
	void OnPostSwingDelayFinished();

	virtual void EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) override; // UGameplayAbility
//~ End Ability

//~ Begin Swing
protected:

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle PreSwing", ReturnDisplayName = "Out PreSwingDelay"))
	float BP_HandlePreSwing();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle BeginSwing", ReturnDisplayName = "Out SwingDuration"))
	float BP_HandleBeginSwing();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle EndSwing", ReturnDisplayName = "Out PostSwingDelay"))
	float BP_HandleEndSwing();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle PostSwing"))
	void BP_HandlePostSwing();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get SwingDamage"))
	float BP_GetSwingDamage() const;

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get SwingDamageTypeClass"))
	TSubclassOf<class UScWDamageType> BP_GetSwingDamageTypeClass() const;

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Apply SwingOwnerEffect"))
	void BP_ApplySwingOwnerEffect();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Remove SwingOwnerEffect"))
	void BP_RemoveSwingOwnerEffect();

	UPROPERTY(Category = "Swing", EditAnywhere, BlueprintReadWrite)
	bool bLoopIfInputIsPressed;

	UPROPERTY(Category = "Swing", Transient, BlueprintReadOnly)
	TObjectPtr<class AScWHandheld_Melee> OwnerMelee;

	UPROPERTY(Category = "Swing", Transient, BlueprintReadOnly)
	TObjectPtr<class UScWHandheldData_Melee> OwnerMeleeData;

	UPROPERTY(Transient)
	FActiveGameplayEffectHandle CurrentSwingEffectHandle;
//~ End Swing

//~ Begin Capture
protected:

	UPROPERTY(Category = "Swing", EditAnywhere, BlueprintReadWrite)
	bool bCaptureDamageDataOnBeginSwingSequence;

	UPROPERTY(Category = "Swing", BlueprintReadWrite)
	float CapturedSwingDamage;

	UPROPERTY(Category = "Swing", BlueprintReadWrite)
	TSubclassOf<class UScWDamageType> CapturedSwingDamageTypeClass;
//~ End Capture
};
