// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAT_WaitOverlap.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskWaitOverlapDelegate, AActor*, InOverlapActor);

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] AT_WaitOverlap"))
class UNREALCOMMONS_API UScWAT_WaitOverlap : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FTaskWaitOverlapDelegate OnOverlap;
	
//~ Begin Initialize
public:

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitOverlap* IDWaitOverlap(UGameplayAbility* InOwningAbility, UPrimitiveComponent* InCheckComponent, UClass* InOverlapClassFilter);

protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

	UFUNCTION()
	void OnOverlapCallback(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, int32 InOtherBodyIndex, bool bFromSweep, const FHitResult& InSweepResult);

	FScriptDelegate OverlapDelegate;

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> CheckComponent;

	UPROPERTY()
	TObjectPtr<UClass> OverlapClassFilter;
};
