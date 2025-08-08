// Scientific Ways

#include "AI/Tasks/ScWBTT_MoveTo_Array.h"

#include "AI/ScWTypes_AI.h"
#include "AI/ScWAIController.h"

#include "Utils/ScWWrapper_VectorArray.h"

UScWBTT_MoveTo_Array::UScWBTT_MoveTo_Array()
{
	NodeName = "Move To (Array)";

	INIT_TASK_NODE_NOTIFY_FLAGS();

	LocationArrayKey.AllowNoneAsValue(false);
	LocationArrayKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, LocationArrayKey), UScWWrapper_VectorArray::StaticClass());
	LocationArrayKey.SelectedKeyName = FScWBlackboard::MoveToLocationArray;

	CurrentArrayIndexKey.AllowNoneAsValue(false);
	CurrentArrayIndexKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, CurrentArrayIndexKey));
	CurrentArrayIndexKey.SelectedKeyName = FScWBlackboard::CurrentArrayIndex;

	NextPointAcceptanceRadiusXY = 32.0f;
	MoveToAcceptanceRadiusXY = 4.0f;
}

//~ Begin Task
void UScWBTT_MoveTo_Array::InitializeFromAsset(UBehaviorTree& InAsset) // UBTNode
{
	Super::InitializeFromAsset(InAsset);

	if (const UBlackboardData* BlackboardAsset = GetBlackboardAsset())
	{
		LocationArrayKey.ResolveSelectedKey(*BlackboardAsset);
		CurrentArrayIndexKey.ResolveSelectedKey(*BlackboardAsset);
	}
	else
	{
		LocationArrayKey.InvalidateResolvedKey();
		CurrentArrayIndexKey.InvalidateResolvedKey();
	}
}

FString UScWBTT_MoveTo_Array::GetStaticDescription() const // UBTNode
{
	return FString::Printf(TEXT("%s: through %s by %s"), *Super::GetStaticDescription(), *LocationArrayKey.SelectedKeyName.ToString(), *CurrentArrayIndexKey.SelectedKeyName.ToString());
}

void UScWBTT_MoveTo_Array::DescribeRuntimeValues(const UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTDescriptionVerbosity::Type InVerbosity, TArray<FString>& OutValues) const // UBTNode
{
	Super::DescribeRuntimeValues(InOwnerTree, InNodeMemory, InVerbosity, OutValues);
}

EBTNodeResult::Type UScWBTT_MoveTo_Array::ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTTaskNode
{
	AAIController* OwnerController = InOwnerTree.GetAIOwner();
	ensureReturn(OwnerController, EBTNodeResult::Failed);

	APawn* OwnerPawn = OwnerController->GetPawn();
	ensureReturn(OwnerPawn, EBTNodeResult::Failed);

	UBlackboardComponent* OwnerBlackboard = InOwnerTree.GetBlackboardComponent();
	ensureReturn(OwnerBlackboard, EBTNodeResult::Failed);

	UScWWrapper_VectorArray* VectorArrayWrapper = Cast<UScWWrapper_VectorArray>(OwnerBlackboard->GetValueAsObject(LocationArrayKey.SelectedKeyName));
	ensureReturn(VectorArrayWrapper, EBTNodeResult::Failed);
	ensureReturn(!VectorArrayWrapper->VectorArray.IsEmpty(), EBTNodeResult::Failed);

	OwnerBlackboard->SetValueAsInt(CurrentArrayIndexKey.SelectedKeyName, 0);
	OwnerController->MoveToLocation(VectorArrayWrapper->VectorArray[OwnerBlackboard->GetValueAsInt(CurrentArrayIndexKey.SelectedKeyName)], MoveToAcceptanceRadiusXY, false);
	return EBTNodeResult::InProgress;
}

void UScWBTT_MoveTo_Array::TickTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) // UBTTaskNode
{
	EBTNodeResult::Type TickResult = HandleTickWithResult(InOwnerTree, InNodeMemory, InDeltaSeconds);

	switch (TickResult)
	{
		case EBTNodeResult::InProgress:
		{
			break;
		}
		default:
		{
			FinishLatentTask(InOwnerTree, TickResult);
			break;
		}
	}
}

UE_DISABLE_OPTIMIZATION

EBTNodeResult::Type UScWBTT_MoveTo_Array::HandleTickWithResult(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds)
{
	AAIController* OwnerController = InOwnerTree.GetAIOwner();
	ensureReturn(OwnerController, EBTNodeResult::Failed);

	APawn* OwnerPawn = OwnerController->GetPawn();
	ensureReturn(OwnerPawn, EBTNodeResult::Failed);

	UBlackboardComponent* OwnerBlackboard = InOwnerTree.GetBlackboardComponent();
	ensureReturn(OwnerBlackboard, EBTNodeResult::Failed);

	UScWWrapper_VectorArray* VectorArrayWrapper = Cast<UScWWrapper_VectorArray>(OwnerBlackboard->GetValueAsObject(LocationArrayKey.SelectedKeyName));
	ensureReturn(VectorArrayWrapper, EBTNodeResult::Failed);
	ensureReturn(!VectorArrayWrapper->VectorArray.IsEmpty(), EBTNodeResult::Failed);

	int32 CurrentIndex = OwnerBlackboard->GetValueAsInt(CurrentArrayIndexKey.SelectedKeyName);
	FVector CurrentLocation = OwnerPawn->GetActorLocation();

	FVector TargetLocation = VectorArrayWrapper->VectorArray[CurrentIndex];

	if (FVector::DistSquaredXY(CurrentLocation, TargetLocation) < NextPointAcceptanceRadiusXY * NextPointAcceptanceRadiusXY)
	{
		++CurrentIndex;

		if (CurrentIndex < VectorArrayWrapper->VectorArray.Num())
		{
			OwnerBlackboard->SetValueAsInt(CurrentArrayIndexKey.SelectedKeyName, CurrentIndex);
			EPathFollowingRequestResult::Type RequestResult = OwnerController->MoveToLocation(VectorArrayWrapper->VectorArray[CurrentIndex], MoveToAcceptanceRadiusXY, false);

			switch (RequestResult)
			{
				case EPathFollowingRequestResult::Failed:
				{
					return EBTNodeResult::Failed;
				}
				case EPathFollowingRequestResult::AlreadyAtGoal:
				{
					return EBTNodeResult::Succeeded;
				}
				case EPathFollowingRequestResult::RequestSuccessful:
				{
					return EBTNodeResult::InProgress;
				}
			}
		}
		else
		{
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::InProgress;
}

UE_ENABLE_OPTIMIZATION

void UScWBTT_MoveTo_Array::OnTaskFinished(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTNodeResult::Type InTaskResult) // UBTTaskNode
{
	Super::OnTaskFinished(InOwnerTree, InNodeMemory, InTaskResult);
}
