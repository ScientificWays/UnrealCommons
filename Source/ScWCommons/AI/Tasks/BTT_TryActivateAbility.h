// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "BTT_TryActivateAbility.generated.h"

/**
 * 
 */
UCLASS(meta = (KeyWords = "TryUseAbility, ActivateAbility"))
class ATA_DREAMCOMETRUE_API UBTT_TryActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTT_TryActivateAbility();

//~ Begin Task
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTTaskNode
//~ End Task

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "AbilityClassKey != nullptr"))
	FGameplayTagContainer AbilityTags;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector AbilityClassKey; // If set, try activate ability by class
//~ End Settings
};
