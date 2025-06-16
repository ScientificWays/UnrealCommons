// G.o.o.P Studios

#include "AI/Tasks/BTT_TryActivateAbility.h"

#include "GAS/ATAASC_Base.h"

UBTT_TryActivateAbility::UBTT_TryActivateAbility()
{
	NodeName = TEXT("Try activate ability");

	AbilityClassKey.AllowNoneAsValue(true);

	TSubclassOf<UObject> AllowedObjectClass = UGameplayAbility::StaticClass();
	AbilityClassKey.AddClassFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, AbilityClassKey), AllowedObjectClass);
}

//~ Begin Task
FString UBTT_TryActivateAbility::GetStaticDescription() const // UBTNode
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

EBTNodeResult::Type UBTT_TryActivateAbility::ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTTaskNode
{
	bool bSucceeded = false;

	if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
	{
		if (APawn* OwnerPawn = OwnerController->GetPawn())
		{
			if (UATAASC_Base* OwnerASC = UATAASC_Base::TryGetBaseAtaASCFromActor(OwnerPawn))
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
