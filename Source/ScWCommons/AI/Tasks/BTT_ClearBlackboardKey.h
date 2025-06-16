// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTT_ClearBlackboardKey.generated.h"

/**
 * 
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UBTT_ClearBlackboardKey : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTT_ClearBlackboardKey();

//~ Begin Task
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTTaskNode
//~ End Task

//~ Begin Settings
protected:

	
//~ End Settings
};
