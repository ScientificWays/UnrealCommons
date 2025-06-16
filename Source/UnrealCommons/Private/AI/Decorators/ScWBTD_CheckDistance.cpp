// Scientific Ways

#include "AI/Decorators/ScWBTD_CheckDistance.h"

#include "AI/ScWTypes_AI.h"
#include "AI/ScWAIFunctionLibrary.h"

UScWBTD_CheckDistance::UScWBTD_CheckDistance()
{
	NodeName = TEXT("Check Distance");

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	FromTargetKey.AllowNoneAsValue(false);
	FromTargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, FromTargetKey));
	FromTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, FromTargetKey), AActor::StaticClass());
	FromTargetKey.SelectedKeyName = FBlackboard::KeySelf;

	ToTargetKey.AllowNoneAsValue(false);
	ToTargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, ToTargetKey));
	ToTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, ToTargetKey), AActor::StaticClass());
	ToTargetKey.SelectedKeyName = FScWBlackboard::KeyTargetActor;

	PassIfLessThanDistance = 500.0f;
	PassIfLessThanDistanceKey.AllowNoneAsValue(true);
	PassIfLessThanDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, PassIfLessThanDistanceKey));

	bHorizontalDistance = false;
	bVerticalDistance = false;
}

//~ Begin Editor
#if WITH_EDITOR
void UScWBTD_CheckDistance::PostEditChangeProperty(FPropertyChangedEvent& InPropertyChangedEvent) // UObject
{
	Super::PostEditChangeProperty(InPropertyChangedEvent);

	if (InPropertyChangedEvent.Property)
	{
		const FName& PropertyName = InPropertyChangedEvent.Property->GetFName();

		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, bHorizontalDistance))
		{
			if (bHorizontalDistance)
			{
				bVerticalDistance = false;
			}
		}
		else if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, bVerticalDistance))
		{
			if (bVerticalDistance)
			{
				bHorizontalDistance = false;
			}
		}
	}
}
#endif // WITH_EDITOR
//~ End Editor

//~ Begin Decorator
FString UScWBTD_CheckDistance::GetStaticDescription() const // UBTNode
{
	FString FromDescription = TEXT("(invalid)");
	FString ToDescription = TEXT("(invalid)");

	if (FromTargetKey.IsSet())
	{
		FromDescription = FromTargetKey.SelectedKeyName.ToString();
	}
	if (ToTargetKey.IsSet())
	{
		ToDescription = ToTargetKey.SelectedKeyName.ToString();
	}
	FString DistanceDescription;

	if (PassIfLessThanDistanceKey.IsNone())
	{
		DistanceDescription = FString::SanitizeFloat(PassIfLessThanDistance);
	}
	else
	{
		DistanceDescription = PassIfLessThanDistanceKey.SelectedKeyName.ToString();
	}

	if (bHorizontalDistance)
	{
		DistanceDescription += TEXT(" (horizontal)");
	}
	else if (bVerticalDistance)
	{
		DistanceDescription += TEXT(" (vertical)");
	}
	return FString::Printf(TEXT("From %s to %s\nCheck distance: %s"), *FromDescription, *ToDescription, *DistanceDescription);
}

void UScWBTD_CheckDistance::InitializeFromAsset(UBehaviorTree& InTreeAsset) // UBTNode
{
	Super::InitializeFromAsset(InTreeAsset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		FromTargetKey.ResolveSelectedKey(*BlackboardData);
		ToTargetKey.ResolveSelectedKey(*BlackboardData);
		PassIfLessThanDistanceKey.ResolveSelectedKey(*BlackboardData);
	}
}

bool UScWBTD_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const // UBTDecorator
{
	AAIController* OwnerController = InOwnerTree.GetAIOwner();
	UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent();

	if (OwnerController && BlackboardComponent)
	{
		FVector FromLocation, ToLocation;
		
		if (!BlackboardComponent->GetLocationFromEntry(FromTargetKey.GetSelectedKeyID(), FromLocation)
			|| !BlackboardComponent->GetLocationFromEntry(ToTargetKey.GetSelectedKeyID(), ToLocation))
		{
			return false;
		}
		float SampleSquaredDistance = 0.0f;

		if (bHorizontalDistance)
		{
			SampleSquaredDistance = (ToLocation - FromLocation).SizeSquared2D();
		}
		else if (bVerticalDistance)
		{
			SampleSquaredDistance = FMath::Square(ToLocation.Z - FromLocation.Z);
		}
		else
		{
			SampleSquaredDistance = (ToLocation - FromLocation).SizeSquared();
		}
		return SampleSquaredDistance < FMath::Square(PassIfLessThanDistanceKey.IsSet() ? BlackboardComponent->GetValueAsFloat(PassIfLessThanDistanceKey.SelectedKeyName) : PassIfLessThanDistance);
	}
	return false;
}
//~ End Decorator
