// Scientific Ways

#include "AI/Services/ScWBTS_TurnTo.h"

#include "AI/ScWTypes_AI.h"
#include "AI/ScWAIController.h"
#include "AI/Tasks/ScWBTT_TurnTo.h"

UScWBTS_TurnTo::UScWBTS_TurnTo()
{
	NodeName = "Turn To";

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	Interval = 0.001f;
	RandomDeviation = 0.0f;

	BlackboardKey.AllowNoneAsValue(false);
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey));
	BlackboardKey.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey));
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey), AActor::StaticClass());
	BlackboardKey.SelectedKeyName = FScWBlackboard::KeyTargetActor;

	Offset = FRotator::ZeroRotator;
	ConstantSpeed = -1.0f;
	GoalTolerance = 1.0f;
	bGetRotationFromTarget = false;
	bTurnToPawnViewLocation = true;
	TargetRotationUpdateRate = 0.5f;

#if WITH_EDITORONLY_DATA
	bDrawDebug = false;
#endif // WITH_EDITORONLY_DATA
}

//~ Begin Service
uint16 UScWBTS_TurnTo::GetInstanceMemorySize() const // UBTNode
{
	return sizeof(FThisTaskMemory);
}

FString UScWBTS_TurnTo::GetStaticDescription() const // UBTNode
{
	return UScWBTT_TurnTo::Common_GetStaticDescription(*this, bGetRotationFromTarget, BlackboardKey, Offset, ConstantSpeed);
}

void UScWBTS_TurnTo::DescribeRuntimeValues(const UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTDescriptionVerbosity::Type InVerbosity, TArray<FString>& OutValues) const // UBTNode
{
	Super::DescribeRuntimeValues(InOwnerTree, InNodeMemory, InVerbosity, OutValues);
	UScWBTT_TurnTo::Common_DescribeRuntimeValues(*this, InOwnerTree, InNodeMemory, InVerbosity, OutValues);
}

void UScWBTS_TurnTo::OnBecomeRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTAuxiliaryNode
{
	if (AScWAIController* OwnerController = Cast<AScWAIController>(InOwnerTree.GetAIOwner()))
	{
		UScWBTT_TurnTo::Common_Init(*this, InOwnerTree, InNodeMemory);

		FThisTaskMemory* ThisNodeMemory = CastInstanceNodeMemory<FThisTaskMemory>(InNodeMemory);
		ThisNodeMemory->UpdateTimeLeft = TargetRotationUpdateRate;

		UScWBTT_TurnTo::Common_UpdateTargetRotation(*this, InOwnerTree, InNodeMemory, BlackboardKey, Offset, bGetRotationFromTarget, bTurnToPawnViewLocation);
	}
}

void UScWBTS_TurnTo::OnCeaseRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTAuxiliaryNode
{
	UScWBTT_TurnTo::Common_DeInit(*this, InOwnerTree, InNodeMemory);
	Super::OnCeaseRelevant(InOwnerTree, InNodeMemory);
}

void UScWBTS_TurnTo::TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) // UBTAuxiliaryNode
{
	if (TargetRotationUpdateRate > 0.0f)
	{
		FThisTaskMemory* ThisNodeMemory = CastInstanceNodeMemory<FThisTaskMemory>(InNodeMemory);

		if (ThisNodeMemory->UpdateTimeLeft > InDeltaSeconds)
		{
			ThisNodeMemory->UpdateTimeLeft -= InDeltaSeconds;
		}
		else
		{
			if (UScWBTT_TurnTo::Common_UpdateTargetRotation(*this, InOwnerTree, InNodeMemory, BlackboardKey, Offset, bGetRotationFromTarget, bTurnToPawnViewLocation))
			{
				ThisNodeMemory->UpdateTimeLeft = TargetRotationUpdateRate;
			}
		}
	}
#if WITH_EDITORONLY_DATA
	UScWBTT_TurnTo::Common_UpdateCurrentRotation(*this, InOwnerTree, InNodeMemory, InDeltaSeconds, ConstantSpeed, GoalTolerance, false, bDrawDebug);
#else
	UScWBTT_TurnTo::Common_UpdateCurrentRotation(*this, InOwnerTree, InNodeMemory, InDeltaSeconds, ConstantSpeed, GoalTolerance, false);
#endif // WITH_EDITORONLY_DATA
	Super::TickNode(InOwnerTree, InNodeMemory, InDeltaSeconds);
}
//~ End Service
