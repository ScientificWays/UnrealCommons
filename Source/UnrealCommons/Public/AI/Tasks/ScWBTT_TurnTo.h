// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "AI/ScWTypes_AI.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "ScWBTT_TurnTo.generated.h"

/**
 *	Turn [To] or [Relative To] selected BlackboardKey Target or Owner if not set
 */
UCLASS(meta = (DisplayName = "[ScW] BTT_TurnTo"))
class UNREALCOMMONS_API UScWBTT_TurnTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	struct FThisTaskMemory
	{
		FRotator TargetRotation;
		float UpdateTimeLeft;
	};

public:

	UScWBTT_TurnTo();
	
//~ Begin Task
protected:
	virtual uint16 GetInstanceMemorySize() const override; // UBTNode
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTDescriptionVerbosity::Type InVerbosity, TArray<FString>& OutValues) const override; // UBTNode
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTTaskNode
	virtual void TickTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) override; // UBTTaskNode
	virtual void OnTaskFinished(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTNodeResult::Type InTaskResult) override; // UBTTaskNode
public:
	static FString Common_GetStaticDescription(const UBTNode& InNode, const bool bInGetRotationFromTarget, const FBlackboardKeySelector& InBlackboardKey, const FRotator& InOffset, const float InConstantSpeed);
	static void Common_DescribeRuntimeValues(const UBTNode& InNode, const UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTDescriptionVerbosity::Type InVerbosity, TArray<FString>& OutValues);
	static bool Common_UpdateTargetRotation(UBTNode& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, const FBlackboardKeySelector& InBlackboardKey, const FRotator& InOffset, const bool bInGetRotationFromTarget, const bool bInTurnToPawnViewLocation);
	static EBTNodeResult::Type Common_UpdateCurrentRotation(UBTNode& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds, const float InConstantSpeed, const float InGoalTolerance, const bool bInFinishTaskWhenGoalReached, const bool bInDrawDebug = false);
//~ End Task

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	FRotator Offset;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (ClampMin = "-1.0"))
	float ConstantSpeed;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "bFinishTaskWhenGoalReached", ClampMin = "0.001"))
	float GoalTolerance;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bGetRotationFromTarget;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "!bGetRotationFromTarget"))
	bool bTurnToPawnViewLocation;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (ClampMin = "-1.0"))
	float TargetRotationUpdateRate;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "ConstantSpeed > 0.0"))
	bool bFinishTaskWhenGoalReached;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bDrawDebug;
#endif // WITH_EDITORONLY_DATA
//~ End Settings
};
