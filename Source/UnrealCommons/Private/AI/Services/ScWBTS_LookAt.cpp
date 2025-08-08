// Scientific Ways

#include "AI/Services/ScWBTS_LookAt.h"

#include "AI/ScWTypes_AI.h"
#include "AI/Tasks/ScWBTT_LookAt.h"

UScWBTS_LookAt::UScWBTS_LookAt()
{
	NodeName = TEXT("Look At");

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	BlackboardKey.AllowNoneAsValue(false);
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey));
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey), AActor::StaticClass());
	BlackboardKey.SelectedKeyName = FScWBlackboard::KeyTargetActor;

	FocusPriority = EScWAIFocusPriority::Gameplay3;
	bClearFocusOnCeaseRelevant = true;
	Offset = FRotator::ZeroRotator;
	bLookAtPawnViewLocation = true;
	bFocusOnActor = false;

#if WITH_EDITORONLY_DATA
	bDrawDebug = false;
#endif // WITH_EDITORONLY_DATA 
}

//~ Begin Editor
#if WITH_EDITOR
void UScWBTS_LookAt::PostEditChangeProperty(FPropertyChangedEvent& InPropertyChangedEvent) // UObject
{
	Super::PostEditChangeProperty(InPropertyChangedEvent);

	if (InPropertyChangedEvent.Property)
	{
		const FName& PropertyName = InPropertyChangedEvent.Property->GetFName();

		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, bFocusOnActor))
		{
			bNotifyTick = !bFocusOnActor;
		}
	}
}
#endif // WITH_EDITOR
//~ End Editor

//~ Begin Service
FString UScWBTS_LookAt::GetStaticDescription() const // UBTNode
{
	FString TickDescription = bNotifyTick ? GetStaticTickIntervalDescription() : TEXT("do not tick (actor focus mode)");
	FString TargetDescription = BlackboardKey.IsSet() ? BlackboardKey.SelectedKeyName.ToString() : TEXT("invalid target");
	FString OffsetDescription = (bFocusOnActor || Offset.IsNearlyZero()) ? TEXT("") : FString::Printf(TEXT(" (with offset [P: %.1f, Y: %.1f, R: %.1f])"), Offset.Pitch, Offset.Yaw, Offset.Roll);

	static const UEnum* FocusPriorityEnum = StaticEnum<EScWAIFocusPriority>();
	FString FocusPriorityName = FocusPriorityEnum->GetNameStringByIndex(static_cast<int32>(FocusPriority));

	return FString::Printf(TEXT("Update: %s\nLook at: %s%s\nPriority: %s"), *TickDescription, *TargetDescription, *OffsetDescription, *FocusPriorityName);
}

void UScWBTS_LookAt::InitializeFromAsset(UBehaviorTree& InTreeAsset) // UBTNode
{
	Super::InitializeFromAsset(InTreeAsset);
	bNotifyTick = !bFocusOnActor;
}

void UScWBTS_LookAt::OnBecomeRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTAuxiliaryNode
{
	Super::OnBecomeRelevant(InOwnerTree, InNodeMemory);

	if (bFocusOnActor)
	{
		AAIController* OwnerController = InOwnerTree.GetAIOwner();
		UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent();

		if (OwnerController && BlackboardComponent)
		{
			if (AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(BlackboardKey.SelectedKeyName)))
			{
				OwnerController->SetFocus(TargetActor, static_cast<EBTNodeResult::Type>(FocusPriority));
			}
		}
	}
}

void UScWBTS_LookAt::OnCeaseRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTAuxiliaryNode
{
	Super::OnCeaseRelevant(InOwnerTree, InNodeMemory);

	if (bClearFocusOnCeaseRelevant)
	{
		if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
		{
			OwnerController->ClearFocus(static_cast<EBTNodeResult::Type>(FocusPriority));
		}
	}
}

void UScWBTS_LookAt::TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) // UBTAuxiliaryNode
{
#if WITH_EDITORONLY_DATA
	UScWBTT_LookAt::Common_SetFocalPoint(*this, InOwnerTree, InNodeMemory, BlackboardKey, FocusPriority, Offset, bLookAtPawnViewLocation, bDrawDebug);
#else
	UScWBTT_LookAt::Common_SetFocalPoint(*this, InOwnerTree, InNodeMemory, BlackboardKey, FocusPriority, Offset, bLookAtPawnViewLocation);
#endif // WITH_EDITORONLY_DATA
	Super::TickNode(InOwnerTree, InNodeMemory, InDeltaSeconds);
}
//~ End Service
