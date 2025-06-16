// Scientific Ways

#include "AI/Services/ScWBTS_GetPerceptionTarget.h"

#include "AI/ScWTypes_AI.h"
#include "AI/ScWAIPC_Base.h"
//#include "AI/Filters/AITargetFilter_Base.h"
//#include "AI/Estimators/AITargetEstimator_Base.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"

UScWBTS_GetPerceptionTarget::UScWBTS_GetPerceptionTarget()
{
	NodeName = TEXT("Get Perception Target");

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	bOnlyCurrentlyPerceivedActors = true;
	bUseSightSense = true;
	bUseHearingSense = false;
	bUseDamageSense = false;

	OutRelevantTargetKey.AllowNoneAsValue(false);
	OutRelevantTargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, OutRelevantTargetKey));
	OutRelevantTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, OutRelevantTargetKey), AActor::StaticClass());
	OutRelevantTargetKey.SelectedKeyName = FScWBlackboard::KeyTargetActor;
}

//~ Begin Editor
#if WITH_EDITOR
void UScWBTS_GetPerceptionTarget::PostEditChangeProperty(FPropertyChangedEvent& InPropertyChangedEvent) // UObject
{
	Super::PostEditChangeProperty(InPropertyChangedEvent);

	if (InPropertyChangedEvent.Property && InPropertyChangedEvent.MemberProperty)
	{
		const FName& PropertyName = InPropertyChangedEvent.Property->GetFName();
		const FName& MemberPropertyName = InPropertyChangedEvent.MemberProperty->GetFName();
		const EPropertyChangeType::Type& ChangeType = InPropertyChangedEvent.ChangeType;

		/*if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, TargetFilterArray) && ChangeType == EPropertyChangeType::ValueSet)
		{
			TargetFilterArray.Sort([](const TObjectPtr<UAITargetFilter_Base>& InA, const TObjectPtr<UAITargetFilter_Base>& InB)
			{
				return InA && InB && InA->GetCostValue() <= InB->GetCostValue();
			});
		}
		else if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, TargetEstimatorArray) && ChangeType == EPropertyChangeType::ValueSet)
		{
			TargetEstimatorArray.Sort([](const TObjectPtr<UAITargetEstimator_Base>& InA, const TObjectPtr<UAITargetEstimator_Base>& InB)
			{
				return InA && InB && InA->GetCostValue() <= InB->GetCostValue();
			});
		}*/
	}
}
#endif // WITH_EDITOR
//~ End Editor

//~ Begin Service
void UScWBTS_GetPerceptionTarget::InitializeFromAsset(UBehaviorTree& InTreeAsset) // UBTNode
{
	Super::InitializeFromAsset(InTreeAsset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		OutRelevantTargetKey.ResolveSelectedKey(*BlackboardData);

		/*for (const TObjectPtr<UAITargetFilter_Base>& SampleFilter : TargetFilterArray)
		{
			if (SampleFilter)
			{
				SampleFilter->InitializeFromAsset(InTreeAsset);
			}
		}*/
	}
}

void UScWBTS_GetPerceptionTarget::OnBecomeRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTAuxiliaryNode
{
	Super::OnBecomeRelevant(InOwnerTree, InNodeMemory);

	if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
	{
		if (UScWAIPC_Base* BasePC = Cast<UScWAIPC_Base>(OwnerController->GetPerceptionComponent()))
		{
			BasePC->RegisterGetPerceptionTargetService(*this, InOwnerTree, InNodeMemory);
		}
	}
	OnPerceptionUpdatedCallback(InOwnerTree, InNodeMemory);
}

void UScWBTS_GetPerceptionTarget::OnCeaseRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTAuxiliaryNode
{
	Super::OnCeaseRelevant(InOwnerTree, InNodeMemory);

	if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
	{
		if (UScWAIPC_Base* BasePC = Cast<UScWAIPC_Base>(OwnerController->GetPerceptionComponent()))
		{
			BasePC->UnRegisterGetPerceptionTargetService(InNodeMemory);
		}
	}
}

//UE_DISABLE_OPTIMIZATION

void UScWBTS_GetPerceptionTarget::OnPerceptionUpdatedCallback(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory/*, const TArray<AActor*>& InUpdatedActors*/)
{
	if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
	{
		if (UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent())
		{
			if (APawn* OwnerPawn = OwnerController->GetPawn())
			{
				if (UAIPerceptionComponent* PerceptionComponent = OwnerController->GetPerceptionComponent())
				{
					TArray<AActor*> PerceivedActorArray;

					if (bUseSightSense)
					{
						TArray<AActor*> SightSenseActors;
						bOnlyCurrentlyPerceivedActors
							? PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SightSenseActors)
							: PerceptionComponent->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), SightSenseActors);
						PerceivedActorArray.Append(SightSenseActors);
					}
					if (bUseHearingSense)
					{
						TArray<AActor*> HearingSenseActors;
						bOnlyCurrentlyPerceivedActors
							? PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), HearingSenseActors)
							: PerceptionComponent->GetKnownPerceivedActors(UAISense_Hearing::StaticClass(), HearingSenseActors);
						PerceivedActorArray.Append(HearingSenseActors);
					}
					if (bUseDamageSense)
					{
						TArray<AActor*> DamageSenseActors;
						bOnlyCurrentlyPerceivedActors
							? PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), DamageSenseActors)
							: PerceptionComponent->GetKnownPerceivedActors(UAISense_Damage::StaticClass(), DamageSenseActors);
						PerceivedActorArray.Append(DamageSenseActors);
					}
					if (PerceivedActorArray.IsEmpty())
					{
						BlackboardComponent->ClearValue(OutRelevantTargetKey.SelectedKeyName);
					}
					else
					{
						/*for (const TObjectPtr<UAITargetFilter_Base>& SampleFilter : TargetFilterArray)
						{
							if (SampleFilter)
							{
								SampleFilter->FilterActors(InOwnerTree, PerceivedActorArray);
							}
						}*/
						AActor* TargetActor = nullptr;
						if (PerceivedActorArray.Num() > 0)
						{
							/*TArray<FAITargetSortData> ActorCostArray;
							for (AActor* PerceivedActor : PerceivedActorArray)
							{
								ActorCostArray.Add({ PerceivedActor, 0 });
							}
							for (const TObjectPtr<UAITargetEstimator_Base>& SampleEstimator : TargetEstimatorArray)
							{
								if (SampleEstimator)
								{
									SampleEstimator->EstimateActors(InOwnerTree, ActorCostArray);
								}
							}
							TargetActor = FMath::Max(ActorCostArray).Actor;*/
							TargetActor = PerceivedActorArray[0];
						}
						if (OutRelevantTargetKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
						{
							if (TargetActor)
							{
								FVector TargetLocation = FAISystem::InvalidLocation;

								if (bOnlyCurrentlyPerceivedActors)
								{
									TargetLocation = TargetActor->GetActorLocation();
								}
								else if (const FActorPerceptionInfo* ActorInfo = PerceptionComponent->GetActorInfo(*TargetActor))
								{
									TargetLocation = ActorInfo->GetLastStimulusLocation();
								}
								BlackboardComponent->SetValueAsVector(OutRelevantTargetKey.SelectedKeyName, TargetLocation);
							}
							else
							{
								BlackboardComponent->ClearValue(OutRelevantTargetKey.SelectedKeyName);
							}
						}
						else
						{
							BlackboardComponent->SetValueAsObject(OutRelevantTargetKey.SelectedKeyName, TargetActor);
						}
					}
				}
			}
		}
	}
}
//~ End Service

//UE_ENABLE_OPTIMIZATION
