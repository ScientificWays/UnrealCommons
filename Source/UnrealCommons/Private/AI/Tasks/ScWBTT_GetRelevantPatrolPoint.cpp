// Scientific Ways

#include "AI/Tasks/ScWBTT_GetRelevantPatrolPoint.h"

#include "AI/ScWTypes_AI.h"
#include "AI/ScWAIController.h"
#include "AI/ScWAIPatrolPoint.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UScWBTT_GetRelevantPatrolPoint::UScWBTT_GetRelevantPatrolPoint()
{
	NodeName = TEXT("Get Relevant Patrol Point");

	LocationReferenceKey.AllowNoneAsValue(false);
	LocationReferenceKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, LocationReferenceKey));
	LocationReferenceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, LocationReferenceKey), AActor::StaticClass());
	LocationReferenceKey.SelectedKeyName = FBlackboard::KeySelf;

	DesiredPathPointOffsetKey.AllowNoneAsValue(true);
	DesiredPathPointOffsetKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, DesiredPathPointOffsetKey));

	DesiredPathPointOffset = 1;

	OutLocationKey.AllowNoneAsValue(false);
	OutLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, OutLocationKey));
	OutLocationKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, LocationReferenceKey), AActor::StaticClass());
	OutLocationKey.SelectedKeyName = FScWBlackboard::KeyTargetLocation;
}

//~ Begin Task
FString UScWBTT_GetRelevantPatrolPoint::GetStaticDescription() const // UBTNode
{
	FString LocationReferenceDescription = TEXT("(invalid)");
	FString PathPointDescription = TEXT("(invalid)");

	if (!LocationReferenceKey.IsNone())
	{
		LocationReferenceDescription = LocationReferenceKey.SelectedKeyName.ToString();
	}
	if (DesiredPathPointOffsetKey.IsSet())
	{
		PathPointDescription = DesiredPathPointOffsetKey.SelectedKeyName.ToString();
	}
	else
	{
		PathPointDescription = FString::Printf(TEXT("%d"), DesiredPathPointOffset);
	}
	return FString::Printf(TEXT("Location Reference: %s; Path Point Offset: %s"), *LocationReferenceDescription, *PathPointDescription);
}

void UScWBTT_GetRelevantPatrolPoint::InitializeFromAsset(UBehaviorTree& InTreeAsset) // UBTNode
{
	Super::InitializeFromAsset(InTreeAsset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		LocationReferenceKey.ResolveSelectedKey(*BlackboardData);
		DesiredPathPointOffsetKey.ResolveSelectedKey(*BlackboardData);
		OutLocationKey.ResolveSelectedKey(*BlackboardData);
	}
}

EBTNodeResult::Type UScWBTT_GetRelevantPatrolPoint::ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTTask
{
	if (!LocationReferenceKey.IsSet())
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("%s LocationReferenceKey was not set! Task failed."), *GetName());
		return EBTNodeResult::Failed;
	}
	if (!OutLocationKey.IsSet())
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("%s OutLocationKey was not set! Task failed."), *GetName());
		return EBTNodeResult::Failed;
	}
	AScWAIController* OwnerController = Cast<AScWAIController>(InOwnerTree.GetAIOwner());
	UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent();
	
	if (OwnerController && BlackboardComponent)
	{
		FVector ReferenceLocation = FAISystem::InvalidLocation;

		if (LocationReferenceKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			ReferenceLocation = BlackboardComponent->GetValueAsVector(LocationReferenceKey.SelectedKeyName);
		}
		else if (AActor* CenterActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(LocationReferenceKey.SelectedKeyName)))
		{
			ReferenceLocation = CenterActor->GetActorLocation();
		}
		int32 Offset = DesiredPathPointOffset;

		if (DesiredPathPointOffsetKey.IsSet())
		{
			Offset = BlackboardComponent->GetValueAsInt(DesiredPathPointOffsetKey.SelectedKeyName);
		}
		if (AScWAIPatrolPoint* SamplePatrolPoint = OwnerController->BP_GetRelevantPatrolPoint(ReferenceLocation, Offset))
		{
			if (OutLocationKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
			{
				BlackboardComponent->SetValueAsVector(OutLocationKey.SelectedKeyName, SamplePatrolPoint->BP_GetMoveToLocation());
			}
			else
			{
				BlackboardComponent->SetValueAsObject(OutLocationKey.SelectedKeyName, SamplePatrolPoint);
			}
			return EBTNodeResult::Succeeded;
		}
		else
		{
			BlackboardComponent->ClearValue(OutLocationKey.GetSelectedKeyID());
		}
	}
	return EBTNodeResult::Failed;
}
//~ End Task
