// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWBTD_CheckMovementMode.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] BTD_CheckMovementMode"))
class UNREALCOMMONS_API UScWBTD_CheckMovementMode : public UBTDecorator
{
	GENERATED_BODY()
	
public:

	UScWBTD_CheckMovementMode();

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
