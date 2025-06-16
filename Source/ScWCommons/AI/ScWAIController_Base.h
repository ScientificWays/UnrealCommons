// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ATAAIController_Base.generated.h"

/**
 *
 */
UCLASS()
class ATA_DREAMCOMETRUE_API AATAAIController_Base : public AAIController
{
	GENERATED_BODY()
	
public:	

	AATAAIController_Base(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

//~ Begin Blackboard
protected:
	virtual bool InitializeBlackboard(UBlackboardComponent& InBlackboardComponent, UBlackboardData& InBlackboardAsset) override; // AAIController
//~ End Blackboard

//~ Begin Team
public:
	virtual void SetGenericTeamId(const FGenericTeamId& InNewTeamID) override; // IGenericTeamAgentInterface
//~ End Team

//~ Begin Perception
public:

	UFUNCTION(Category = "Perception", BlueprintCallable, meta = (KeyWords = "GetPerceptionComponent, GetAIPC, GetPC"))
	class UATAAIPC_Base* GetBaseAIPC() const { return BaseAIPC; }

protected:

	UPROPERTY(Category = "Perception", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UATAAIPC_Base> BaseAIPC;
//~ End Perception

//~ Begin Navigation
protected:

	UPROPERTY(Category = "Navigation", BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<class UATAPFC_Base> BasePFC;
//~ End Navigation
	
//~ Begin Rotation
public:

	UFUNCTION(Category = "View", BlueprintCallable)
	void ClearAllFocuses();

	UPROPERTY(Category = "View", EditAnywhere)
	float FocusInterpSpeed;

	virtual FVector GetFocalPointOnActor(const AActor* Actor) const override; // AAIController

	void AddControlRotationBlock(UObject* InObject) { ControlRotationBlockSet.Add(InObject); }
	void RemoveControlRotationBlock(UObject* InObject) { ControlRotationBlockSet.Remove(InObject); }
protected:
	virtual void UpdateControlRotation(float InDeltaSeconds, bool bInUpdatePawn) override; // AAIController

	UPROPERTY(Category = "Pawn", BlueprintReadOnly)
	TSet<UObject*> ControlRotationBlockSet;
//~ End Rotation

};
