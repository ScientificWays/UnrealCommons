// G.o.o.P Studios

#include "AI/ATAAIFunctionLibrary.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionListenerInterface.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

//~ Begin Blackboard
float UATAAIFunctionLibrary::GetDistanceBetweenTwoBlackboardKeys(const UBlackboardComponent* InBlackboard, const FBlackboardKeySelector& InA, const FBlackboardKeySelector& InB, const bool bInSquared, const bool bInHorizontal)
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
	FVector D = LocationB - LocationA;

	if (bInSquared)
	{
		return bInHorizontal ? D.SizeSquared2D() : D.SizeSquared();
	}
	else
	{
		return bInHorizontal ? D.Size2D() : D.Size();
	}
}
//~ End Blackboard

//~ Begin Perception
int32 UATAAIFunctionLibrary::GetActorObserversNumFromActorArray(const AActor* InActor, const TArray<AActor*>& InActorArray)
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
