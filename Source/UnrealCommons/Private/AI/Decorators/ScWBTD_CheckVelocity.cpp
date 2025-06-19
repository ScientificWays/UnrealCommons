// Scientific Ways

#include "AI/Decorators/ScWBTD_CheckVelocity.h"

#include "AI/ScWTypes_AI.h"

UScWBTD_CheckVelocity::UScWBTD_CheckVelocity()
{
	NodeName = TEXT("Check Velocity");

	TargetKey.AllowNoneAsValue(false);
	TargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetKey));
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetKey), AActor::StaticClass());
	TargetKey.SelectedKeyName = FScWBlackboard::KeyTargetActor;

	MinVelocity = 500.0f;

	MinVelocityKey.AllowNoneAsValue(true);
	MinVelocityKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, MinVelocityKey));
}

//~ Begin Decorator
void UScWBTD_CheckVelocity::InitializeFromAsset(UBehaviorTree& InTreeAsset) // UBTNode
{
	Super::InitializeFromAsset(InTreeAsset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		TargetKey.ResolveSelectedKey(*BlackboardData);
		MinVelocityKey.ResolveSelectedKey(*BlackboardData);
	}
}

bool UScWBTD_CheckVelocity::CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const // UBTDecorator
{
	AAIController* OwnerController = InOwnerTree.GetAIOwner();
	UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent();

	if (OwnerController && BlackboardComponent)
	{
		if (AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName)))
		{
			FVector TargetVelocity = TargetActor->GetVelocity();
			float SquaredVelocty = bCheckHorizontalVelocityOnly ? TargetVelocity.SizeSquared() : TargetVelocity.SizeSquared2D();

			float CheckVelocity = MinVelocity;

			if (MinVelocityKey.IsSet())
			{
				CheckVelocity = BlackboardComponent->GetValueAsFloat(MinVelocityKey.SelectedKeyName);
			}
			return SquaredVelocty > FMath::Square(CheckVelocity);
		}
	}
	return false;
}
//~ End Decorator
