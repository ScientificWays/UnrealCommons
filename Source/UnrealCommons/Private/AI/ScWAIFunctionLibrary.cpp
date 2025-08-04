// Scientific Ways

#include "AI/ScWAIFunctionLibrary.h"

#include "AI/ScWAIPatrolPoint.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionListenerInterface.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

//~ Begin Blackboard
float UScWAIFunctionLibrary::GetDistanceBetweenTwoBlackboardKeys(const UBlackboardComponent* InBlackboard, const FBlackboardKeySelector& InA, const FBlackboardKeySelector& InB, const bool bInSquared, const bool bInHorizontal)
{
	FVector LocationA = FAISystem::InvalidLocation;
	FVector LocationB = FAISystem::InvalidLocation;

	if (InA.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		LocationA = InBlackboard->GetValueAsVector(InA.SelectedKeyName);
	}
	else if (AActor* ActorA = Cast<AActor>(InBlackboard->GetValueAsObject(InA.SelectedKeyName)))
	{
		LocationA = ActorA->GetActorLocation();
	}
	if (InB.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		LocationB = InBlackboard->GetValueAsVector(InB.SelectedKeyName);
	}
	else if (AActor* ActorB = Cast<AActor>(InBlackboard->GetValueAsObject(InB.SelectedKeyName)))
	{
		LocationB = ActorB->GetActorLocation();
	}
	FVector DistanceVector = LocationB - LocationA;

	if (bInSquared)
	{
		return bInHorizontal ? DistanceVector.SizeSquared2D() : DistanceVector.SizeSquared();
	}
	else
	{
		return bInHorizontal ? DistanceVector.Size2D() : DistanceVector.Size();
	}
}
//~ End Blackboard

//~ Begin Perception
int32 UScWAIFunctionLibrary::GetActorObserversNumFromActorArray(const AActor* InActor, const TArray<AActor*>& InActorArray)
{
	int32 OutObserversNum = 0;
	const FAISenseID SenseId = UAISense::GetSenseID(UAISense_Sight::StaticClass());

	for (const AActor* SampleActor : InActorArray)
	{
		if (const IAIPerceptionListenerInterface* SampleListener = Cast<IAIPerceptionListenerInterface>(SampleActor))
		{
			UAIPerceptionComponent* PerceptionComponent = const_cast<IAIPerceptionListenerInterface*>(SampleListener)->GetPerceptionComponent();
			check(PerceptionComponent);

			if (PerceptionComponent->HasActiveStimulus(*InActor, SenseId))
			{
				++OutObserversNum;
			}
		}
	}
	return OutObserversNum;
}
//~ End Perception

//~ Begin Navigation
int32 UScWAIFunctionLibrary::GetNearestPatrolPointIndex(const FVector& InReferenceLocation, const TArray<AScWAIPatrolPoint*>& InPatrolPoints, const bool bInUseMoveToLocations)
{
	float NearestDistanceSquared = MAX_FLT;
	int32 OutIndex = INDEX_NONE;

	for (int32 SampleIndex = 0; SampleIndex < InPatrolPoints.Num(); ++SampleIndex)
	{
		const AScWAIPatrolPoint* SamplePoint = InPatrolPoints[SampleIndex];
		FVector SampleLocation = bInUseMoveToLocations ? SamplePoint->BP_GetMoveToLocation() : SamplePoint->GetActorLocation();
		float SampleDistanceSquared = FVector::DistSquared(InReferenceLocation, SampleLocation);

		if (SampleDistanceSquared < NearestDistanceSquared)
		{
			OutIndex = SampleIndex;
			NearestDistanceSquared = SampleDistanceSquared;
		}
	}
	return OutIndex;
}

AScWAIPatrolPoint* UScWAIFunctionLibrary::GetNearestPatrolPoint(const FVector& InReferenceLocation, const TArray<AScWAIPatrolPoint*>& InPatrolPoints, const bool bInUseMoveToLocations)
{
	int32 OutIndex = GetNearestPatrolPointIndex(InReferenceLocation, InPatrolPoints, bInUseMoveToLocations);
	return InPatrolPoints.IsValidIndex(OutIndex) ? InPatrolPoints[OutIndex] : nullptr;
}

FVector UScWAIFunctionLibrary::GetAIMoveDirection(AAIController* InController, FVector InFallbackDirection)
{
	ensureReturn(InController, InFallbackDirection);

	const UPathFollowingComponent* TargetPFC = InController->GetPathFollowingComponent();
	ensureReturn(TargetPFC, InFallbackDirection);

	const auto& NavPath = TargetPFC->GetPath();
	ensureReturn(NavPath.IsValid(), InFallbackDirection);

	const auto& PathPoints = NavPath->GetPathPoints();
	if (PathPoints.Num() < 2)
	{
		return InFallbackDirection;
	}
	APawn* TargetPawn = InController->GetPawn();
	ensureReturn(TargetPawn, InFallbackDirection);

	const auto& CurrentLocation = TargetPawn->GetActorLocation();
	const auto& NextLocation = PathPoints[1].Location;
	return (NextLocation - CurrentLocation).GetSafeNormal();
}
//~ End Navigation


//~ Begin Brain
UBrainComponent* UScWAIFunctionLibrary::TryGetActorBrainComponent(const AActor* InActor)
{
	if (const APawn* PawnActor = Cast<APawn>(InActor))
	{
		if (const AController* PawnController = PawnActor->GetController())
		{
			return PawnController->FindComponentByClass<UBrainComponent>();
		}
	}
	return InActor->FindComponentByClass<UBrainComponent>();
}
//~ End Brain