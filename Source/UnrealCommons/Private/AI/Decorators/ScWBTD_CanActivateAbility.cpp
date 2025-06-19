// Scientific Ways

#include "AI/Decorators/ScWBTD_CanActivateAbility.h"

UScWBTD_CanActivateAbility::UScWBTD_CanActivateAbility()
{
	NodeName = TEXT("Can Activate Ability");

	//bNotifyBecomeRelevant = true;

	AbilityClassKey.AllowNoneAsValue(true);

	TSubclassOf<UObject> AllowedObjectClass = UGameplayAbility::StaticClass();
	AbilityClassKey.AddClassFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, AbilityClassKey), AllowedObjectClass);

	//bTryCacheAbilityOnBecomeRelevant = true;
}

//~ Begin Decorator
/*uint16 UScWBTD_CanActivateAbility::GetInstanceMemorySize() const // UBTNode
{
	return sizeof(FThisTaskMemory);
}*/

FString UScWBTD_CanActivateAbility::GetStaticDescription() const // UBTNode
{
	FString AbilityData = TEXT("with ");

	if (AbilityClassKey.IsSet())
	{
		AbilityData += (AbilityClassKey.SelectedKeyName.ToString() + TEXT(" class"));
	}
	else
	{
		AbilityData += AbilityTags.ToStringSimple(true) + TEXT(" tags");
	}
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *AbilityData);
}

/*void UScWBTD_CanActivateAbility::OnBecomeRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTAuxiliaryNode
{
	Super::OnBecomeRelevant(InOwnerTree, InNodeMemory);

	if (bTryCacheAbilityOnBecomeRelevant)
	{
		if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
		{
			if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerController->GetPawn()))
			{
				FThisTaskMemory* ThisNodeMemory = CastInstanceNodeMemory<FThisTaskMemory>(InNodeMemory);
				ThisNodeMemory->AbilitySpec = TryGetAbilitySpec(InOwnerTree, OwnerASC);
			}
		}
	}
}*/

bool UScWBTD_CanActivateAbility::CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const // UBTDecorator
{
	if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
	{
		if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerController->GetPawn()))
		{
			/*FThisTaskMemory* ThisNodeMemory = CastInstanceNodeMemory<FThisTaskMemory>(InNodeMemory);
			check(ThisNodeMemory);
			
			if (FGameplayAbilitySpec* AbilitySpec = ThisNodeMemory->AbilitySpec ? ThisNodeMemory->AbilitySpec : TryGetAbilitySpec(InOwnerTree, OwnerASC))
			{
				return AbilitySpec->Ability->CanActivateAbility(AbilitySpec->Handle, OwnerASC->AbilityActorInfo.Get());
			}*/
			if (const FGameplayAbilitySpec* AbilitySpec = TryGetAbilitySpec(InOwnerTree, OwnerASC))
			{
				return AbilitySpec->Ability->CanActivateAbility(AbilitySpec->Handle, OwnerASC->AbilityActorInfo.Get());
			}
		}
	}
	return false;
}

FGameplayAbilitySpec* UScWBTD_CanActivateAbility::TryGetAbilitySpec(const UBehaviorTreeComponent& InOwnerTree, const UAbilitySystemComponent* FromASC) const
{
	if (const UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent())
	{
		if (AbilityClassKey.IsSet())
		{
			return const_cast<UAbilitySystemComponent*>(FromASC)->FindAbilitySpecFromClass(BlackboardComponent->GetValueAsClass(AbilityClassKey.SelectedKeyName));
		}
		else
		{
			TArray<FGameplayAbilitySpec*> MatchingAbilitySpecs;
			FromASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, MatchingAbilitySpecs);
			return MatchingAbilitySpecs.IsEmpty() ? nullptr : MatchingAbilitySpecs[0];
		}
	}
	return nullptr;
}
//~ End Decorator
