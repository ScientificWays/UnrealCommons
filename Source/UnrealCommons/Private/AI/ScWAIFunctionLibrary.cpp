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
int32 UScWAIFunctionLibrary::GetNearestPatrolPointIndex(const FVector& InReferenceLocation, const TArray<AScWAIPatrolPoint*>& InPatrolPoints, const bool bInCheckMoveToLocations)
{
	float NearestDistanceSquared = MAX_FLT;
	int32 OutIndex = INDEX_NONE;

	for (int32 SampleIndex = 0; SampleIndex < InPatrolPoints.Num(); ++SampleIndex)
	{
		const AScWAIPatrolPoint* SamplePoint = InPatrolPoints[SampleIndex];
		FVector SampleLocation = bInCheckMoveToLocations ? SamplePoint->BP_GetMoveToLocation() : SamplePoint->GetActorLocation();
		float SampleDistanceSquared = FVector::DistSquared(InReferenceLocation, SampleLocation);

		if (SampleDistanceSquared < NearestDistanceSquared)
		{
			OutIndex = SampleIndex;
			NearestDistanceSquared = SampleDistanceSquared;
		}
	}
	return OutIndex;
}

AScWAIPatrolPoint* UScWAIFunctionLibrary::GetNearestPatrolPoint(const FVector& InReferenceLocation, const TArray<AScWAIPatrolPoint*>& InPatrolPoints, const bool bInCheckMoveToLocations)
{
	int32 OutIndex = GetNearestPatrolPointIndex(InReferenceLocation, InPatrolPoints, bInCheckMoveToLocations);
	return InPatrolPoints.IsValidIndex(OutIndex) ? InPatrolPoints[OutIndex] : nullptr;
}
//~ End Navigation
