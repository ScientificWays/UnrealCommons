// Scientific Ways

#include "AI/Tasks/ScWBTT_TurnTo.h"

#include "AI/ScWTypes_AI.h"
#include "AI/ScWAIController.h"

UScWBTT_TurnTo::UScWBTT_TurnTo()
{
	NodeName = "Turn To";

	INIT_TASK_NODE_NOTIFY_FLAGS();

	BlackboardKey.AllowNoneAsValue(false);
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey));
	BlackboardKey.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey));
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey), AActor::StaticClass());
	BlackboardKey.SelectedKeyName = FScWBlackboard::KeyTargetActor;

	Offset = FRotator::ZeroRotator;
	ConstantSpeed = -1.0f;
	GoalTolerance = 1.0f;
	bGetRotationFromTarget = false;
	bTurnToPawnViewLocation = true;
	TargetRotationUpdateRate = -1.0f;
	bFinishTaskWhenGoalReached = true;

#if WITH_EDITORONLY_DATA
	bDrawDebug = false;
#endif // WITH_EDITORONLY_DATA
}

//~ Begin Task
uint16 UScWBTT_TurnTo::GetInstanceMemorySize() const // UBTNode
{
	return sizeof(FThisTaskMemory);
}

FString UScWBTT_TurnTo::GetStaticDescription() const // UBTNode
{
	return Common_GetStaticDescription(*this, bGetRotationFromTarget, BlackboardKey, Offset, ConstantSpeed);
}

void UScWBTT_TurnTo::DescribeRuntimeValues(const UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTDescriptionVerbosity::Type InVerbosity, TArray<FString>& OutValues) const // UBTNode
{
	Super::DescribeRuntimeValues(InOwnerTree, InNodeMemory, InVerbosity, OutValues);
	Common_DescribeRuntimeValues(*this, InOwnerTree, InNodeMemory, InVerbosity, OutValues);
}

EBTNodeResult::Type UScWBTT_TurnTo::ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTTaskNode
{
	if (AScWAIController* OwnerController = Cast<AScWAIController>(InOwnerTree.GetAIOwner()))
	{
		Common_Init(*this, InOwnerTree, InNodeMemory);

		FThisTaskMemory* ThisNodeMemory = CastInstanceNodeMemory<FThisTaskMemory>(InNodeMemory);
		ThisNodeMemory->UpdateTimeLeft = TargetRotationUpdateRate;

		Common_UpdateTargetRotation(*this, InOwnerTree, InNodeMemory, BlackboardKey, Offset, bGetRotationFromTarget, bTurnToPawnViewLocation);

		if (bFinishTaskWhenGoalReached && ThisNodeMemory->TargetRotation.Equals(OwnerController->GetControlRotation(), GoalTolerance))
		{
			return EBTNodeResult::Succeeded;
		}
		if (ConstantSpeed > 0.0f)
		{
			return EBTNodeResult::InProgress;
		}
		else
		{
#if WITH_EDITORONLY_DATA
			return Common_UpdateCurrentRotation(*this, InOwnerTree, InNodeMemory, 0.0f, ConstantSpeed, GoalTolerance, bFinishTaskWhenGoalReached, bDrawDebug);
#else
			return Common_UpdateCurrentRotation(*this, InOwnerTree, InNodeMemory, 0.0f, ConstantSpeed, GoalTolerance, bFinishTaskWhenGoalReached);
#endif // WITH_EDITORONLY_DATA
		}
	}
	return EBTNodeResult::Failed;
}

void UScWBTT_TurnTo::TickTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) // UBTTaskNode
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
			if (Common_UpdateTargetRotation(*this, InOwnerTree, InNodeMemory, BlackboardKey, Offset, bGetRotationFromTarget, bTurnToPawnViewLocation))
			{
				ThisNodeMemory->UpdateTimeLeft = TargetRotationUpdateRate;
			}
			else
			{
				FinishLatentTask(InOwnerTree, EBTNodeResult::Failed);
				return;
			}
		}
	}
#if WITH_EDITORONLY_DATA
	EBTNodeResult::Type TickResult = Common_UpdateCurrentRotation(*this, InOwnerTree, InNodeMemory, InDeltaSeconds, ConstantSpeed, GoalTolerance, bFinishTaskWhenGoalReached, bDrawDebug);
#else
	EBTNodeResult::Type TickResult = Common_UpdateCurrentRotation(*this, InOwnerTree, InNodeMemory, InDeltaSeconds, ConstantSpeed, GoalTolerance, bFinishTaskWhenGoalReached);
#endif // WITH_EDITORONLY_DATA
	if (TickResult != EBTNodeResult::InProgress)
	{
		FinishLatentTask(InOwnerTree, TickResult);
	}
}

void UScWBTT_TurnTo::OnTaskFinished(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTNodeResult::Type InTaskResult) // UBTTaskNode
{
	Common_DeInit(*this, InOwnerTree, InNodeMemory);
	Super::OnTaskFinished(InOwnerTree, InNodeMemory, InTaskResult);
}

//UE_DISABLE_OPTIMIZATION

void UScWBTT_TurnTo::Common_Init(UBTNode& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory)
{
	if (AScWAIController* OwnerController = Cast<AScWAIController>(InOwnerTree.GetAIOwner()))
	{
		OwnerController->AddControlRotationUpdateBlockSource(&InNode);
		OwnerController->ForceControlRotationOnPawn();
	}
}

void UScWBTT_TurnTo::Common_DeInit(UBTNode& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory)
{
	if (AScWAIController* OwnerController = Cast<AScWAIController>(InOwnerTree.GetAIOwner()))
	{
		OwnerController->RemoveControlRotationUpdateBlockSource(&InNode);
		OwnerController->DisableForceControlRotationOnPawn();
	}
}

FString UScWBTT_TurnTo::Common_GetStaticDescription(const UBTNode& InNode, const bool bInGetRotationFromTarget, const FBlackboardKeySelector& InBlackboardKey, const FRotator& InOffset, const float InConstantSpeed)
{
	FString ActionDescription = bInGetRotationFromTarget ? TEXT("In direction as") : TEXT("Turn to");
	FString TargetDescription = TEXT("Owner");
	FString OffsetDescription = TEXT("");
	FString DelayDescription = TEXT("Immediately");

	if (InBlackboardKey.IsSet())
	{
		TargetDescription = InBlackboardKey.SelectedKeyName.ToString();
	}
	if (!InOffset.IsNearlyZero(1e-3f))
	{
		OffsetDescription = FString::Printf(TEXT(" (with offset [P: %.1f, Y: %.1f, R: %.1f])"), InOffset.Pitch, InOffset.Yaw, InOffset.Roll);
	}
	if (InConstantSpeed > 0.0f)
	{
		DelayDescription = FString::Printf(TEXT("Speed: %.1f"), InConstantSpeed);
	}
	return FString::Printf(TEXT("%s: %s%s\n%s"), *ActionDescription, *TargetDescription, *OffsetDescription, *DelayDescription);
}

void UScWBTT_TurnTo::Common_DescribeRuntimeValues(const UBTNode& InNode, const UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTDescriptionVerbosity::Type InVerbosity, TArray<FString>& OutValues)
{
	if (const AAIController* OwnerController = InOwnerTree.GetAIOwner())
	{
		FRotator CurrentRotation = OwnerController->GetControlRotation();

		FThisTaskMemory* ThisNodeMemory = InNode.CastInstanceNodeMemory<FThisTaskMemory>(InNodeMemory);
		FRotator Delta = (ThisNodeMemory->TargetRotation - CurrentRotation).GetNormalized();

		OutValues.Add(FString::Printf(TEXT("Delta: [P: %.1f, Y: %.1f, R: %.1f]"), Delta.Pitch, Delta.Yaw, Delta.Roll));
	}
}

bool UScWBTT_TurnTo::Common_UpdateTargetRotation(UBTNode& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, const FBlackboardKeySelector& InBlackboardKey, const FRotator& InOffset, const bool bInGetRotationFromTarget, const bool bInTurnToPawnViewLocation)
{
	AScWAIController* OwnerController = Cast<AScWAIController>(InOwnerTree.GetAIOwner());
	UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent();

	if (OwnerController && BlackboardComponent)
	{
		if (APawn* OwnerPawn = OwnerController->GetPawn())
		{
			FThisTaskMemory* ThisNodeMemory = InNode.CastInstanceNodeMemory<FThisTaskMemory>(InNodeMemory);

			if (bInGetRotationFromTarget)
			{
				FRotator InitialRotation = FRotator::ZeroRotator;

				if (InBlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
				{
					InitialRotation = BlackboardComponent->GetValueAsRotator(InBlackboardKey.SelectedKeyName);
				}
				else if (InBlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
				{
					InitialRotation = BlackboardComponent->GetValueAsVector(InBlackboardKey.SelectedKeyName).Rotation();
				}
				else if (APawn* TargetPawn = Cast<APawn>(BlackboardComponent->GetValueAsObject(InBlackboardKey.SelectedKeyName)))
				{
					InitialRotation = TargetPawn->GetControlRotation();
				}
				else if (AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(InBlackboardKey.SelectedKeyName)))
				{
					InitialRotation = TargetActor->GetActorRotation();
				}
				else
				{
					ensure(false);
					InitialRotation = OwnerController->GetControlRotation();
				}
				ThisNodeMemory->TargetRotation = FRotator(FQuat(InOffset) * FQuat(InitialRotation));
			}
			else
			{
				FVector OwnerViewLocation = OwnerPawn->GetPawnViewLocation();
				FVector TargetLocation = FAISystem::InvalidLocation;

				if (InBlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
				{
					TargetLocation = BlackboardComponent->GetValueAsVector(InBlackboardKey.SelectedKeyName);
				}
				else if (APawn* TargetPawn = Cast<APawn>(BlackboardComponent->GetValueAsObject(InBlackboardKey.SelectedKeyName)))
				{
					TargetLocation = bInTurnToPawnViewLocation ? TargetPawn->GetPawnViewLocation() : TargetPawn->GetActorLocation();
				}
				else if (AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(InBlackboardKey.SelectedKeyName)))
				{
					TargetLocation = TargetActor->GetActorLocation();
				}
				else
				{
					ensure(false);
					return false;
				}
				ThisNodeMemory->TargetRotation = InOffset.RotateVector(TargetLocation - OwnerViewLocation).ToOrientationRotator();
			}
			return true;
		}
	}
	ensure(false);
	return false;
}

EBTNodeResult::Type UScWBTT_TurnTo::Common_UpdateCurrentRotation(UBTNode& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds, const float InConstantSpeed, const float InGoalTolerance, const bool bInFinishTaskWhenGoalReached, const bool bInDrawDebug)
{
	AAIController* OwnerController = InOwnerTree.GetAIOwner();
	UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent();

	if (OwnerController && BlackboardComponent)
	{
		if (APawn* OwnerPawn = OwnerController->GetPawn())
		{
			FRotator CurrentRotation = OwnerController->GetControlRotation();

			FThisTaskMemory* ThisNodeMemory = InNode.CastInstanceNodeMemory<FThisTaskMemory>(InNodeMemory);
			FRotator Delta = (ThisNodeMemory->TargetRotation - CurrentRotation).GetNormalized();

			FRotator NewRotation = FRotator::ZeroRotator;

			if (InConstantSpeed > 0.0f)
			{
				const float D = InConstantSpeed * InDeltaSeconds;
				Delta.Pitch = FMath::Clamp(Delta.Pitch, -D, D);
				Delta.Yaw = FMath::Clamp(Delta.Yaw, -D, D);
				Delta.Roll = FMath::Clamp(Delta.Roll, -D, D);

				NewRotation = (CurrentRotation + Delta).GetNormalized();
			}
			else
			{
				NewRotation = ThisNodeMemory->TargetRotation;
			}
			OwnerController->SetControlRotation(NewRotation);

#if WITH_EDITORONLY_DATA
			if (bInDrawDebug)
			{
				FVector OwnerViewLocation = OwnerPawn->GetPawnViewLocation();
				DrawDebugLine(InNode.GetWorld(), OwnerViewLocation, OwnerViewLocation + NewRotation.Vector() * 500.0f, FColor::Red, false, InDeltaSeconds * 3.0f);
				DrawDebugLine(InNode.GetWorld(), OwnerViewLocation, OwnerViewLocation + ThisNodeMemory->TargetRotation.Vector() * 500.0f, FColor::Blue, false, InConstantSpeed ? 2.0f : InDeltaSeconds * 3.0f, 0, 1.0f);
			}
#endif // WITH_EDITORONLY_DATA
			return (bInFinishTaskWhenGoalReached && NewRotation.Equals(ThisNodeMemory->TargetRotation, InGoalTolerance)) ? EBTNodeResult::Succeeded : EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
}
//~ End Task

//UE_ENABLE_OPTIMIZATION
