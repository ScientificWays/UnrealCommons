// Scientific Ways

#include "AI/Services/ScWBTS_GetRandomReachablePointInRadius.h"

#include "AI/ScWTypes_AI.h"
#include "AI/Tasks/ScWBTT_GetRandomReachablePointInRadius.h"

UScWBTS_GetRandomReachablePointInRadius::UScWBTS_GetRandomReachablePointInRadius()
{
	NodeName = TEXT("Get Random Reachable Point in Radius");

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	CenterKey.AllowNoneAsValue(false);
	CenterKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, CenterKey));
	CenterKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, CenterKey), AActor::StaticClass());
	CenterKey.SelectedKeyName = FBlackboard::KeySelf;

	RadiusKey.AllowNoneAsValue(true);
	RadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, RadiusKey));

	Radius = 500.0f;

	OutLocationKey.AllowNoneAsValue(false);
	OutLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, OutLocationKey));
	OutLocationKey.SelectedKeyName = FScWBlackboard::KeyTargetLocation;
}

//~ Begin Service
FString UScWBTS_GetRandomReachablePointInRadius::GetStaticDescription() const // UBTNode
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
	return FString::Printf(TEXT("Center: %s Radius: %s"), *CenterDescription, *RadiusDescription);
}

void UScWBTS_GetRandomReachablePointInRadius::InitializeFromAsset(UBehaviorTree& InTreeAsset) // UBTNode
{
	Super::InitializeFromAsset(InTreeAsset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		CenterKey.ResolveSelectedKey(*BlackboardData);
		RadiusKey.ResolveSelectedKey(*BlackboardData);
		OutLocationKey.ResolveSelectedKey(*BlackboardData);
	}
}

void UScWBTS_GetRandomReachablePointInRadius::TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) // UBTAuxiliaryNode
{
	UScWBTT_GetRandomReachablePointInRadius::Common_Execute(*this, InOwnerTree, InNodeMemory, CenterKey, RadiusKey, Radius, OutLocationKey);
	Super::TickNode(InOwnerTree, InNodeMemory, InDeltaSeconds);
}
//~ End Service
