// G.o.o.P Studios

#include "AI/Tasks/BTT_GetRandomReachablePointInRadius.h"

#include "AI/Types_AI.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTT_GetRandomReachablePointInRadius::UBTT_GetRandomReachablePointInRadius()
{
	NodeName = TEXT("Get Random Reachable Point in Radius");

	CenterKey.AllowNoneAsValue(false);
	CenterKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, CenterKey), AActor::StaticClass());
	CenterKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, CenterKey));
	CenterKey.SelectedKeyName = FBlackboard::KeySelf;

	RadiusKey.AllowNoneAsValue(true);
	RadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, RadiusKey));

	Radius = 500.0f;

	OutLocationKey.AllowNoneAsValue(false);
	OutLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, OutLocationKey));
	OutLocationKey.SelectedKeyName = FATABlackboard::KeyTargetLocation;
}

//~ Begin Task
FString UBTT_GetRandomReachablePointInRadius::GetStaticDescription() const // UBTNode
{
	FString CenterDescription = TEXT("(invalid)");
	FString RadiusDescription = TEXT("(invalid)");

	if (!CenterKey.IsNone())
	{
		CenterDescription = CenterKey.SelectedKeyName.ToString();
	}
	if (RadiusKey.IsSet())
	{
		RadiusDescription = RadiusKey.SelectedKeyName.ToString();
	}
	else
	{
		RadiusDescription = FString::Printf(TEXT("%.1f"), Radius);
	}
	return FString::Printf(TEXT("Center: %s; Radius %s"), *CenterDescription, *RadiusDescription);
}

void UBTT_GetRandomReachablePointInRadius::InitializeFromAsset(UBehaviorTree& InTreeAsset) // UBTNode
{
	Super::InitializeFromAsset(InTreeAsset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		CenterKey.ResolveSelectedKey(*BlackboardData);
		RadiusKey.ResolveSelectedKey(*BlackboardData);
		OutLocationKey.ResolveSelectedKey(*BlackboardData);
	}
}

EBTNodeResult::Type UBTT_GetRandomReachablePointInRadius::ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTTask
{
	return Common_Execute(*this, InOwnerTree, InNodeMemory, CenterKey, RadiusKey, Radius, OutLocationKey);
}

EBTNodeResult::Type UBTT_GetRandomReachablePointInRadius::Common_Execute(UBTNode& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, const FBlackboardKeySelector& InCenterKey, const FBlackboardKeySelector& InRadiusKey, float InRadius, const FBlackboardKeySelector& InOutLocationKey)
{
	if (!InCenterKey.IsSet())
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("%s CenterKey was not set! Task failed."), *InNode.GetName());
		return EBTNodeResult::Failed;
	}
	if (!InOutLocationKey.IsSet())
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("%s OutLocationKey was not set! Task failed."), *InNode.GetName());
		return EBTNodeResult::Failed;
	}
	AAIController* OwnerController = InOwnerTree.GetAIOwner();
	UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent();
	UNavigationSystemV1* NavigationSystem = Cast<UNavigationSystemV1>(InNode.GetWorld()->GetNavigationSystem());

	if (!NavigationSystem)
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("%s NavigationSystem is invalid! Task failed."), *InNode.GetName());
		return EBTNodeResult::Failed;
	}
	if (OwnerController && BlackboardComponent)
	{
		FVector CenterLocation = FVector::ZeroVector;

		if (InCenterKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			CenterLocation = BlackboardComponent->GetValueAsVector(InCenterKey.SelectedKeyName);
		}
		else if (AActor* CenterActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(InCenterKey.SelectedKeyName)))
		{
			CenterLocation = CenterActor->GetActorLocation();
		}
		float Radius = InRadius;

		if (InRadiusKey.IsSet())
		{
			Radius = BlackboardComponent->GetValueAsFloat(InRadiusKey.SelectedKeyName);
		}
		FNavLocation SampleNavPoint;
		if (NavigationSystem->GetRandomReachablePointInRadius(CenterLocation, Radius, SampleNavPoint))
		{
			BlackboardComponent->SetValueAsVector(InOutLocationKey.SelectedKeyName, SampleNavPoint.Location);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			BlackboardComponent->ClearValue(InOutLocationKey.GetSelectedKeyID());
		}
	}
	return EBTNodeResult::Failed;
}
//~ End Task
