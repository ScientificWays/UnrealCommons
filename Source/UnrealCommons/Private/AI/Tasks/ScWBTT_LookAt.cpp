// Scientific Ways

#include "AI/Tasks/ScWBTT_LookAt.h"

#include "AI/ScWTypes_AI.h"

UScWBTT_LookAt::UScWBTT_LookAt()
{
	NodeName = "Look At";

	BlackboardKey.AllowNoneAsValue(false);
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey));
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey), AActor::StaticClass());
	BlackboardKey.SelectedKeyName = FScWBlackboard::KeyTargetActor;

	FocusPriority = EScWAIFocusPriority::Gameplay3;
	Offset = FRotator::ZeroRotator;
	bLookAtPawnViewLocation = false;

#if WITH_EDITORONLY_DATA
	bDrawDebug = false;
#endif // WITH_EDITORONLY_DATA
}

//~ Begin Task
FString UScWBTT_LookAt::GetStaticDescription() const // UBTNode
{
	FString TargetDescription = BlackboardKey.IsSet() ? BlackboardKey.SelectedKeyName.ToString() : TEXT("invalid target");
	FString OffsetDescription = (bFocusOnActor || Offset.IsNearlyZero()) ? TEXT("") : FString::Printf(TEXT(" (with offset [P: %.1f, Y: %.1f, R: %.1f])"), Offset.Pitch, Offset.Yaw, Offset.Roll);

	static const UEnum* FocusPriorityEnum = StaticEnum<EScWAIFocusPriority>();
	FString FocusPriorityName = FocusPriorityEnum->GetNameStringByIndex(static_cast<int32>(FocusPriority));

	return FString::Printf(TEXT("Look at: %s%s\nPriority: %s"), *TargetDescription, *OffsetDescription, *FocusPriorityName);
}

EBTNodeResult::Type UScWBTT_LookAt::ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTTaskNode
{
	AAIController* OwnerController = InOwnerTree.GetAIOwner();
	UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent();

	if (OwnerController && BlackboardComponent)
	{
		if (bFocusOnActor)
		{
			if (AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(BlackboardKey.SelectedKeyName)))
			{
				OwnerController->SetFocus(TargetActor, static_cast<EBTNodeResult::Type>(FocusPriority));
				return EBTNodeResult::Succeeded;
			}
		}
		else
		{
#if WITH_EDITORONLY_DATA
			return Common_SetFocalPoint(*this, InOwnerTree, InNodeMemory, BlackboardKey, FocusPriority, Offset, bLookAtPawnViewLocation, bDrawDebug);
#else
			return Common_SetFocalPoint(*this, InOwnerTree, InNodeMemory, BlackboardKey, FocusPriority, Offset, bLookAtPawnViewLocation);
#endif // WITH_EDITORONLY_DATA
		}
	}
	return EBTNodeResult::Failed;
}

//UE_DISABLE_OPTIMIZATION

EBTNodeResult::Type UScWBTT_LookAt::Common_SetFocalPoint(UBTNode& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, const FBlackboardKeySelector& InBlackboardKey, const EScWAIFocusPriority InFocusPriority, const FRotator& InOffset, const bool bInLookAtPawnViewLocation, const bool bInDrawDebug)
{
	AAIController* OwnerController = InOwnerTree.GetAIOwner();
	UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent();

	if (OwnerController && BlackboardComponent)
	{
		if (APawn* OwnerPawn = OwnerController->GetPawn())
		{
			FVector OwnerViewLocation = OwnerPawn->GetPawnViewLocation();
			FRotator OwnerViewRotation = OwnerController->GetControlRotation();

			FVector FocusLocation = FAISystem::InvalidLocation;

			if (!InBlackboardKey.IsSet())
			{
				FocusLocation = OwnerViewLocation + InOffset.Vector() * 1000.0f; // Rare case, but provide at least this kind of focus
			}
			else 
			{
				if (InBlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
				{
					FocusLocation = BlackboardComponent->GetValueAsVector(InBlackboardKey.SelectedKeyName);
				}
				else
				{
					if (APawn* TargetPawn = Cast<APawn>(BlackboardComponent->GetValueAsObject(InBlackboardKey.SelectedKeyName)))
					{
						FocusLocation = bInLookAtPawnViewLocation ? TargetPawn->GetPawnViewLocation() : TargetPawn->GetActorLocation();
					}
					else if (AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(InBlackboardKey.SelectedKeyName)))
					{
						FocusLocation = TargetActor->GetActorLocation();
					}
				}
				if (FocusLocation != FAISystem::InvalidLocation)
				{
					FocusLocation = OwnerViewLocation + InOffset.RotateVector(FocusLocation - OwnerViewLocation);
				}
			}
#if WITH_EDITORONLY_DATA
			if (bInDrawDebug)
			{
				DrawDebugLine(InNode.GetWorld(), OwnerViewLocation, FocusLocation, FColor::Red, false, 2.0f, 0, 2.0f);
			}
#endif // WITH_EDITORONLY_DATA
			OwnerController->SetFocalPoint(FocusLocation, static_cast<EAIFocusPriority::Type>(InFocusPriority));
			return FocusLocation == FAISystem::InvalidLocation ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
//~ End Task

//UE_ENABLE_OPTIMIZATION
