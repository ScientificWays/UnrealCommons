// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAT_WaitHit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskWaitHitSignature, AActor*, InHitActor);

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] AT_WaitHit"))
class UNREALCOMMONS_API UScWAT_WaitHit : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FTaskWaitHitSignature OnHit;
	
//~ Begin Initialize
public:

	UFUNCTION(Category = "Ability | Tasks", BlueprintCallable, meta = (DisplayName = "[ScW] WaitHit", HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitHit* WaitHit(UGameplayAbility* InOwningAbility, UPrimitiveComponent* InCheckComponent, UClass* InHitClassFilter);

	UFUNCTION(Category = "Ability | Tasks", BlueprintCallable, meta = (DisplayName = "[ScW] WaitHit", HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitHit* WaitHit_IgnoreGround(UGameplayAbility* InOwningAbility, UPrimitiveComponent* InCheckComponent, UClass* InHitClassFilter, UCharacterMovementComponent* InCharacterMovementComponent);

protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Hit
protected:

	UFUNCTION()
	void OnHitCallback(UPrimitiveComponent* InHitComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, FVector InNormalImpulse, const FHitResult& InHit);

	FScriptDelegate HitDelegate;

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> CheckComponent;

	UPROPERTY()
	TObjectPtr<UClass> HitClassFilter;
	
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	bool bIgnoreMovementBase;
//~ End Hit
};
