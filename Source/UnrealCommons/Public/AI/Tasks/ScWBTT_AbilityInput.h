// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/ScWTypes_Gameplay.h"

#include "ScWBTT_AbilityInput.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] BTT_AbilityInput", KeyWords = "TryUseAbility, TryActivateAbility"))
class UNREALCOMMONS_API UScWBTT_AbilityInput : public UBTTaskNode
{
	GENERATED_BODY()

	struct FThisTaskMemory
	{
		class UAbilitySystemComponent* OwnerASC;
	};

public:

	UScWBTT_AbilityInput();

//~ Begin Task
protected:
	virtual uint16 GetInstanceMemorySize() const override; // UBTNode
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTDescriptionVerbosity::Type InVerbosity, TArray<FString>& OutValues) const override; // UBTNode
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTTaskNode
	virtual void TickTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) override; // UBTTaskNode
	virtual void OnTaskFinished(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTNodeResult::Type InTaskResult) override; // UBTTaskNode
//~ End Task

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	EScWAbilityInputID InputID;

	// Press Input on TaskExecute
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bPressInput;

	// Release Input on TaskExecute
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bReleaseInput;

	// Release Input delay after TaskExecute
	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "bReleaseInput", ClampMin = "0.0"))
	float ReleaseInputDelay;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bForceReleaseInputOnTaskFinished;
//~ End Settings
};
