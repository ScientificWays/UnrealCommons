// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "BTD_CheckMovementMode.generated.h"

/**
 * 
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UBTD_CheckMovementMode : public UBTDecorator
{
	GENERATED_BODY()
	
public:

	UBTD_CheckMovementMode();

//~ Begin Decorator
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTAuxiliaryNode
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTAuxiliaryNode
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const override; // UBTDecorator

	UFUNCTION()
	void OnMovementModeChangedCallback(ACharacter* InCharacter, EMovementMode InPreviousMovementMode, uint8 InPreviousCustomMode);
//~ End Decorator

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	TEnumAsByte<EMovementMode> RequiredMode;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "RequiredMode == EMovementMode::MOVE_Custom", EditConditionHides))
	uint8 RequiredCustomMode;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bNotifyObserverOnMovementModeChanged;
//~ End Settings
};
