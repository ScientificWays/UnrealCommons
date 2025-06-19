// Scientific Ways

#include "AI/Services/ScWBTS_GetDistanceTo.h"

#include "AI/ScWAIFunctionLibrary.h"

UScWBTS_GetDistanceTo::UScWBTS_GetDistanceTo()
{
	NodeName = TEXT("Get Distance To");

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	FromTargetKey.AllowNoneAsValue(false);
	FromTargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, FromTargetKey));
	FromTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, FromTargetKey), AActor::StaticClass());

	ToTargetKey.AllowNoneAsValue(false);
	ToTargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, ToTargetKey));
	ToTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, ToTargetKey), AActor::StaticClass());

	OutDistanceKey.AllowNoneAsValue(false);
	OutDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, OutDistanceKey));

	bGetSquaredDistance = false;
	bGetHorizontalDistance = false;
	bGetVerticalDistance = false;
}

//~ Begin Editor
#if WITH_EDITOR
void UScWBTS_GetDistanceTo::PostEditChangeProperty(FPropertyChangedEvent& InPropertyChangedEvent) // UObject
{
	Super::PostEditChangeProperty(InPropertyChangedEvent);

	if (InPropertyChangedEvent.Property)
	{
		const FName& PropertyName = InPropertyChangedEvent.Property->GetFName();

		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, bGetHorizontalDistance))
		{
			if (bGetHorizontalDistance)
			{
				bGetVerticalDistance = false;
			}
		}
		else if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, bGetVerticalDistance))
		{
			if (bGetVerticalDistance)
			{
				bGetHorizontalDistance = false;
			}
		}
	}
}
#endif // WITH_EDITOR
//~ End Editor

//~ Begin Service
void UScWBTS_GetDistanceTo::InitializeFromAsset(UBehaviorTree& InTreeAsset) // UBTNode
{
	Super::InitializeFromAsset(InTreeAsset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		FromTargetKey.ResolveSelectedKey(*BlackboardData);
		ToTargetKey.ResolveSelectedKey(*BlackboardData);
		OutDistanceKey.ResolveSelectedKey(*BlackboardData);
	}
}

void UScWBTS_GetDistanceTo::TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) // UBTAuxiliaryNode
{
	UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent();

	if (BlackboardComponent)
	{
		FVector FromLocation, ToLocation;

		if (!BlackboardComponent->GetLocationFromEntry(FromTargetKey.GetSelectedKeyID(), FromLocation)
		|| !BlackboardComponent->GetLocationFromEntry(ToTargetKey.GetSelectedKeyID(), ToLocation))
		{
			return;
		}
		FVector DistanceVector = ToLocation - FromLocation;
		float Distance = -1.0f;

		if (bGetHorizontalDistance)
		{
			Distance = bGetSquaredDistance ? DistanceVector.SizeSquared2D() : DistanceVector.Size2D();
		}
		else if (bGetVerticalDistance)
		{
			Distance = bGetSquaredDistance ? FMath::Square(DistanceVector.Z) : FMath::Abs(DistanceVector.Z);
		}
		else
		{
			Distance = bGetSquaredDistance ? DistanceVector.SizeSquared() : DistanceVector.Size();
		}
		BlackboardComponent->SetValueAsFloat(OutDistanceKey.SelectedKeyName, Distance);
	}
	Super::TickNode(InOwnerTree, InNodeMemory, InDeltaSeconds);
}
//~ End Service
