// Scientific Ways

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ATAAT_ModifyCharacterMovement.generated.h"

/**
 * 
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UATAAT_ModifyCharacterMovement : public UAbilityTask
{
	GENERATED_BODY()

public:

	UATAAT_ModifyCharacterMovement();
	
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Time Out"))
	FGenericGameplayTaskDelegate OnTimeOutExec;

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UATAAT_ModifyCharacterMovement* ModifyCharacterMovement(UGameplayAbility* InOwningAbility, class UATACMC_Base* InTarget, FVector InMagnitude, const bool bInApplyAsForce = false, float InTimeOut = -1.0f);
	
//~ Begin Initialize
protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Tick
protected:
	virtual void TickTask(float InDeltaSeconds) override; // UGameplayTask
//~ End Tick

//~ Begin TimeOut
protected:
	void OnTimeOut();
//~ End TimeOut

	FVector Magnitude;
	bool bApplyAsForce;

	FTimerHandle TimeOutTimerHandle;
	float TimeOut;

	UPROPERTY()
	TObjectPtr<class UATACMC_Base> TargetCMC;
};
