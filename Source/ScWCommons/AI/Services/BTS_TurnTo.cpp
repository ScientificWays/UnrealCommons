// G.o.o.P Studios

#include "AI/Services/BTS_TurnTo.h"

#include "AI/Types_AI.h"
#include "AI/ATAAIController_Base.h"
#include "AI/Tasks/BTT_TurnTo.h"

UBTS_TurnTo::UBTS_TurnTo()
{
	NodeName = "Turn to";

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	Interval = 0.001f;
	RandomDeviation = 0.0f;

	BlackboardKey.AllowNoneAsValue(false);
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey));
	BlackboardKey.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey));
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey), AActor::StaticClass());
	BlackboardKey.SelectedKeyName = FATABlackboard::KeyTargetActor;

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
uint16 UBTS_TurnTo::GetInstanceMemorySize() const // UBTNode
{
	return sizeof(FThisTaskMemory);
}

FString UBTS_TurnTo::GetStaticDescription() const // UBTNode
{
	return UBTT_TurnTo::Common_GetStaticDescription(*this, bGetRotationFromTarget, BlackboardKey, Offset, ConstantSpeed);
}

void UBTS_TurnTo::DescribeRuntimeValues(const UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTDescriptionVerbosity::Type InVerbosity, TArray<FString>& OutValues) const // UBTNode
{
	Super::DescribeRuntimeValues(InOwnerTree, InNodeMemory, InVerbosity, OutValues);
	UBTT_TurnTo::Common_DescribeRuntimeValues(*this, InOwnerTree, InNodeMemory, InVerbosity, OutValues);
}

void UBTS_TurnTo::OnBecomeRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTAuxiliaryNode
{
	if (AATAAIController_Base* OwnerController = Cast<AATAAIController_Base>(InOwnerTree.GetAIOwner()))
	{
		OwnerController->AddControlRotationBlock(this);

		FThisTaskMemory* ThisNodeMemory = CastInstanceNodeMemory<FThisTaskMemory>(InNodeMemory);
		ThisNodeMemory->UpdateTimeLeft = TargetRotationUpdateRate;

		UBTT_TurnTo::Common_UpdateTargetRotation(*this, InOwnerTree, InNodeMemory, BlackboardKey, Offset, bGetRotationFromTarget, bTurnToPawnViewLocation);
	}
}

void UBTS_TurnTo::OnCeaseRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTAuxiliaryNode
{
	Super::OnCeaseRelevant(InOwnerTree, InNodeMemory);
	
	if (AATAAIController_Base* OwnerController = Cast<AATAAIController_Base>(InOwnerTree.GetAIOwner()))
	{
		OwnerController->RemoveControlRotationBlock(this);
	}
}

void UBTS_TurnTo::TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) // UBTAuxiliaryNode
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
			if (UBTT_TurnTo::Common_UpdateTargetRotation(*this, InOwnerTree, InNodeMemory, BlackboardKey, Offset, bGetRotationFromTarget, bTurnToPawnViewLocation))
			{
				ThisNodeMemory->UpdateTimeLeft = TargetRotationUpdateRate;
			}
		}
	}
#if WITH_EDITORONLY_DATA
	UBTT_TurnTo::Common_UpdateCurrentRotation(*this, InOwnerTree, InNodeMemory, InDeltaSeconds, ConstantSpeed, GoalTolerance, false, bDrawDebug);
#else
	UBTT_TurnTo::Common_UpdateCurrentRotation(*this, InOwnerTree, InNodeMemory, InDeltaSeconds, ConstantSpeed, GoalTolerance, false);
#endif // WITH_EDITORONLY_DATA
	Super::TickNode(InOwnerTree, InNodeMemory, InDeltaSeconds);
}
//~ End Service
