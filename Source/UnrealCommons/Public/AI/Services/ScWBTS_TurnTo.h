// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "BehaviorTree/Services/BTService_BlackboardBase.h"

#include "ScWBTS_TurnTo.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOMMONS_API UScWBTS_TurnTo : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	struct FThisTaskMemory
	{
		FRotator TargetRotation;
		float UpdateTimeLeft;
	};

public:

	UScWBTS_TurnTo();
	
//~ Begin Service
protected:
	virtual uint16 GetInstanceMemorySize() const override; // UBTNode
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTDescriptionVerbosity::Type InVerbosity, TArray<FString>& OutValues) const override; // UBTNode
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTAuxiliaryNode
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTAuxiliaryNode
	virtual void TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) override; // UBTAuxiliaryNode
//~ End Service

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	FRotator Offset;

	/* Turn immediately if less than or equals zero */
	UPROPERTY(Category = "Settings", EditAnywhere, meta = (ClampMin = "-1.0"))
	float ConstantSpeed;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (ClampMin = "0.001"))
	float GoalTolerance;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bGetRotationFromTarget;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "!bGetRotationFromTarget"))
	bool bTurnToPawnViewLocation;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (ClampMin = "-1.0"))
	float TargetRotationUpdateRate;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bDrawDebug;
#endif // WITH_EDITORONLY_DATA
//~ End Settings
};
