// Scientific Ways

#include "AI/Tasks/ScWBTT_TryActivateAbility.h"

#include "Gameplay/ScWASC_Base.h"

UScWBTT_TryActivateAbility::UScWBTT_TryActivateAbility()
{
	NodeName = TEXT("Try activate ability");

	AbilityClassKey.AllowNoneAsValue(true);

	TSubclassOf<UObject> AllowedObjectClass = UGameplayAbility::StaticClass();
	AbilityClassKey.AddClassFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, AbilityClassKey), AllowedObjectClass);
}

//~ Begin Task
FString UScWBTT_TryActivateAbility::GetStaticDescription() const // UBTNode
{
	FString AbilityData = TEXT("with ");

	if (AbilityClassKey.IsSet())
	{
		AbilityData += (AbilityClassKey.SelectedKeyName.ToString() + TEXT(" input"));
	}
	else
	{
		AbilityData += AbilityTags.ToStringSimple(true) + TEXT(" tags");
	}
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *AbilityData);
}

EBTNodeResult::Type UScWBTT_TryActivateAbility::ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTTaskNode
{
	bool bSucceeded = false;

	if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
	{
		if (APawn* OwnerPawn = OwnerController->GetPawn())
		{
			if (UScWASC_Base* OwnerASC = UScWASC_Base::TryGetBaseAtaASCFromActor(OwnerPawn))
			{
				if (AbilityClassKey.IsSet())
				{
					if (const UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent())
					{
						bSucceeded = OwnerASC->TryActivateAbilityByClass(BlackboardComponent->GetValueAsClass(AbilityClassKey.SelectedKeyName));
					}
				}
				else
				{
					bSucceeded = OwnerASC->TryActivateAbilitiesByTag(AbilityTags);
				}
			}
		}
	}
	return bSucceeded ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
//~ End Task
