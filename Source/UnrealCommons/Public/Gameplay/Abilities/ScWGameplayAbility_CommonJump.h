// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Abilities/ScWGameplayAbility.h"

#include "ScWGameplayAbility_CommonJump.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Common Jump Gameplay Ability"))
class UNREALCOMMONS_API UScWGameplayAbility_CommonJump : public UScWGameplayAbility
{

	GENERATED_BODY()

public:

	UScWGameplayAbility_CommonJump();

//~ Begin Ability
protected:
	virtual void NativeActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) override; // UGameplayAbility
	virtual void EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) override; // UGameplayAbility
//~ End Ability

//~ Begin Jump
public:

	UPROPERTY(Category = "Jump", EditAnywhere, BlueprintReadWrite)
	bool bStopJumpingOnAbilityEnd;

protected:

	UFUNCTION()
	void OnPlayerLandedCallback(const FHitResult& InHitResult);

	UFUNCTION()
	void OnAbilityInputReleaseCallback(float InTimeHeld);
//~ End Jump
};
