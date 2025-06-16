// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWBTS_GetPerceptionTarget.generated.h"

UENUM(BlueprintType)
enum class ERelevantEnemy_FilterRule : uint8
{
	Nearest,
	Random,
};

/**
 * 
 */
UCLASS()
class UNREALCOMMONS_API UScWBTS_GetPerceptionTarget : public UBTService
{
	GENERATED_BODY()

public:

	UScWBTS_GetPerceptionTarget();
	
//~ Begin Editor
#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& InPropertyChangedEvent) override; // UObject
#endif // WITH_EDITOR	
//~ End Editor

//~ Begin Service
protected:
	virtual void InitializeFromAsset(UBehaviorTree& InTreeAsset) override; // UBTNode
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTAuxiliaryNode
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTAuxiliaryNode
public:
	virtual void OnPerceptionUpdatedCallback(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory/*, const TArray<AActor*>& InUpdatedActors*/);
//~ End Service

//~ Begin Settings
protected:

	// If false, consider all unexpired sense data, not only actively perceived actors
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bOnlyCurrentlyPerceivedActors;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bUseSightSense;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bUseHearingSense;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bUseDamageSense;

	// Filters to apply (from the first to the last) to perceived actors. Array gets sorted in optimal order after setting an element.
	//UPROPERTY(Category = "Settings", EditAnywhere, Instanced)
	//TArray<TObjectPtr<class UAITargetFilter_Base>> TargetFilterArray;

	//UPROPERTY(Category = "Settings", EditAnywhere, Instanced)
	//TArray<TObjectPtr<class UAITargetEstimator_Base>> TargetEstimatorArray;
//~ End Settings

//~ Begin Output
protected:

	UPROPERTY(Category = "Output", EditAnywhere)
	FBlackboardKeySelector OutRelevantTargetKey;
//~ End Output
};
