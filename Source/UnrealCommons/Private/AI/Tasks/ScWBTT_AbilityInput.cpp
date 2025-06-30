// Scientific Ways

#include "AI/Tasks/ScWBTT_AbilityInput.h"

UScWBTT_AbilityInput::UScWBTT_AbilityInput()
{
	NodeName = TEXT("Ability Input");

	bTickIntervals = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();

	bPressInput = true;
	bReleaseInput = true;
	ReleaseInputDelay = 0.0f;
	bForceReleaseInputOnTaskFinished = false;
}

//~ Begin Task
uint16 UScWBTT_AbilityInput::GetInstanceMemorySize() const // UBTNode
{
	return sizeof(FThisTaskMemory);
}

FString UScWBTT_AbilityInput::GetStaticDescription() const // UBTNode
{
	FString InputType = TEXT("Do Nothing");

	if (bPressInput && bReleaseInput)
	{
		InputType = TEXT("Press and Release");
	}
	else if (bPressInput)
	{
		InputType = TEXT("Press");
	}
	else if (bReleaseInput)
	{
		InputType = TEXT("Release");
	}
	static const UEnum* AbilityInputEnum = StaticEnum<EScWAbilityInputID>();
	FString InputName = AbilityInputEnum->GetNameStringByIndex(static_cast<int32>(InputID));

	FString InputReleaseDelay = FString();
	if (ReleaseInputDelay > 0.0f)
	{
		InputReleaseDelay = FString::Printf(TEXT(" for %.1fs"), ReleaseInputDelay);
	}
	return FString::Printf(TEXT("%s: %s%s"), *InputType, *InputName, *InputReleaseDelay);
}

void UScWBTT_AbilityInput::DescribeRuntimeValues(const UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTDescriptionVerbosity::Type InVerbosity, TArray<FString>& OutValues) const // UBTNode
{
	Super::DescribeRuntimeValues(InOwnerTree, InNodeMemory, InVerbosity, OutValues);

	FBTTaskMemory* TaskCommonMemory = GetSpecialNodeMemory<FBTTaskMemory>(InNodeMemory);
	OutValues.Add(FString::Printf(TEXT("Release delay: %.1f"), TaskCommonMemory->NextTickRemainingTime));
}

EBTNodeResult::Type UScWBTT_AbilityInput::ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTTaskNode
{
	if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
	{
		if (APawn* OwnerPawn = OwnerController->GetPawn())
		{
			if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerPawn))
			{
				if (bPressInput)
				{
					OwnerASC->PressInputID(static_cast<int32>(InputID));
					return EBTNodeResult::Succeeded;
				}
				if (bReleaseInput)
				{
					if (ReleaseInputDelay > 0.0f)
					{
						FThisTaskMemory* ThisNodeMemory = CastInstanceNodeMemory<FThisTaskMemory>(InNodeMemory);
						ThisNodeMemory->OwnerASC = OwnerASC;

						SetNextTickTime(InNodeMemory, ReleaseInputDelay);
						return EBTNodeResult::InProgress;
					}
					else
					{
						OwnerASC->ReleaseInputID(static_cast<int32>(InputID));
					}
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UScWBTT_AbilityInput::TickTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) // UBTTaskNode
{
	FThisTaskMemory* ThisNodeMemory = CastInstanceNodeMemory<FThisTaskMemory>(InNodeMemory);
	if (ThisNodeMemory->OwnerASC)
	{
		ThisNodeMemory->OwnerASC->ReleaseInputID(static_cast<int32>(InputID));
	}
	FinishLatentTask(InOwnerTree, EBTNodeResult::Succeeded);
}

void UScWBTT_AbilityInput::OnTaskFinished(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTNodeResult::Type InTaskResult) // UBTTaskNode
{
	if (bForceReleaseInputOnTaskFinished)
	{
		FThisTaskMemory* ThisNodeMemory = CastInstanceNodeMemory<FThisTaskMemory>(InNodeMemory);		
		if (ThisNodeMemory->OwnerASC)
		{
			ThisNodeMemory->OwnerASC->ReleaseInputID(static_cast<int32>(InputID));
		}
	}
	Super::OnTaskFinished(InOwnerTree, InNodeMemory, InTaskResult);
}
//~ End Task
