// Scientific Ways

#include "AI/Decorators/ScWBTD_CheckMovementMode.h"

UScWBTD_CheckMovementMode::UScWBTD_CheckMovementMode()
{
	NodeName = TEXT("Check Movement Mode");

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	bCreateNodeInstance = true;

	RequiredMode = EMovementMode::MOVE_Walking;
	RequiredCustomMode = 0u;

	bNotifyObserverOnMovementModeChanged = true;
}

//~ Begin Decorator
FString UScWBTD_CheckMovementMode::GetStaticDescription() const // UBTNode
{
	static const UEnum* MovementModeEnum = StaticEnum<EMovementMode>();
	FString MovementModeDescription = MovementModeEnum->GetNameStringByIndex(static_cast<int32>(RequiredMode));

	FString CustomMovementModeDescription = TEXT("");
	if (RequiredCustomMode)
	{
		CustomMovementModeDescription = FString::Printf(TEXT("(%u)"), RequiredCustomMode);
	}
	return FString::Printf(TEXT("%s\nCheck mode: %s%s"), *Super::GetStaticDescription(), *MovementModeDescription, *CustomMovementModeDescription);
}

void UScWBTD_CheckMovementMode::OnBecomeRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTDecorator
{
	Super::OnBecomeRelevant(InOwnerTree, InNodeMemory);

	if (bNotifyObserverOnMovementModeChanged)
	{
		if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
		{
			if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerController->GetPawn()))
			{
				OwnerCharacter->MovementModeChangedDelegate.RemoveDynamic(this, &UScWBTD_CheckMovementMode::OnMovementModeChangedCallback);
			}
		}
	}
}

void UScWBTD_CheckMovementMode::OnCeaseRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTDecorator
{
	Super::OnCeaseRelevant(InOwnerTree, InNodeMemory);

	if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerController->GetPawn()))
		{
			OwnerCharacter->MovementModeChangedDelegate.RemoveDynamic(this, &UScWBTD_CheckMovementMode::OnMovementModeChangedCallback);
		}
	}
}

bool UScWBTD_CheckMovementMode::CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const // UBTDecorator
{
	if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
	{
		if (ACharacter* OwnerCharacter = OwnerController->GetPawn<ACharacter>())
		{
			if (UCharacterMovementComponent* OwnerCMC = OwnerCharacter->GetCharacterMovement())
			{
				if (OwnerCMC->MovementMode == RequiredMode)
				{
					return RequiredMode != EMovementMode::MOVE_Custom || OwnerCMC->CustomMovementMode == RequiredCustomMode;
				}
			}
		}
	}
	return false;
}

void UScWBTD_CheckMovementMode::OnMovementModeChangedCallback(ACharacter* InCharacter, EMovementMode InPreviousMovementMode, uint8 InPreviousCustomMode)
{
	if (AAIController* CharacterController = InCharacter->GetController<AAIController>())
	{
		if (UBehaviorTreeComponent* CharacterTree = Cast<UBehaviorTreeComponent>(CharacterController->BrainComponent))
		{
			CharacterTree->RequestExecution(this);
		}
	}
}
//~ End Decorator
