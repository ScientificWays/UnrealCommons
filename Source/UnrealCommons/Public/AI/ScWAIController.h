// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAIController.generated.h"

/**
 *
 */
UCLASS(Abstract, meta = (DisplayName = "[ScW] AI Controller"))
class UNREALCOMMONS_API AScWAIController : public AAIController, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	

	AScWAIController(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

//~ Begin Blackboard
protected:
	virtual bool InitializeBlackboard(UBlackboardComponent& InBlackboardComponent, UBlackboardData& InBlackboardAsset) override; // AAIController
//~ End Blackboard

//~ Begin Team
public:

	UFUNCTION(Category = "Team", BlueprintCallable)
	FGenericTeamId GetTeamId() const { return GetGenericTeamId(); }

	virtual void SetGenericTeamId(const FGenericTeamId& InNewTeamID) override; // IGenericTeamAgentInterface
//~ End Team

//~ Begin Perception
public:

	UFUNCTION(Category = "Perception", BlueprintCallable, meta = (KeyWords = "GetPerceptionComponent, GetAIPC, GetPC"))
	class UScWAIPC_Base* GetBaseAIPC() const { return BaseAIPC; }

protected:

	UPROPERTY(Category = "Perception", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UScWAIPC_Base> BaseAIPC;
//~ End Perception

//~ Begin Navigation
public:

	UFUNCTION(Category = "Navigation | Patrol", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetRelevantPatrolPoint"))
	class AScWAIPatrolPoint* BP_GetRelevantPatrolPoint(const FVector& InReferenceLocation, int32 InDesiredPathPointOffset) const;

protected:

	UPROPERTY(Category = "Navigation", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UScWPFC_Base> BasePFC;

	UPROPERTY(Category = "Navigation | Patrol", EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class AScWAIPatrolPoint>> CurrentPatrolPoints;
//~ End Navigation
	
//~ Begin AbilitySystem
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface
//~ End AbilitySystem

//~ Begin Rotation
public:

	UFUNCTION(Category = "Rotation", BlueprintCallable)
	void ClearAllFocuses();

	UPROPERTY(Category = "Rotation", EditAnywhere, BlueprintReadWrite)
	float FocusInterpSpeed;

	UFUNCTION(Category = "Rotation", BlueprintCallable)
	void ForceControlRotationOnPawn();

	UFUNCTION(Category = "Rotation", BlueprintCallable)
	void DisableForceControlRotationOnPawn();

	virtual FVector GetFocalPointOnActor(const AActor* Actor) const override; // AAIController

	UFUNCTION(Category = "Rotation", BlueprintCallable)
	void AddControlRotationUpdateBlockSource(UObject* InObject) { ControlRotationUpdateBlockSet.Add(InObject); }

	UFUNCTION(Category = "Rotation", BlueprintCallable)
	void RemoveControlRotationUpdateBlockSource(UObject* InObject) { ControlRotationUpdateBlockSet.Remove(InObject); }

protected:
	virtual void UpdateControlRotation(float InDeltaSeconds, bool bInUpdatePawn) override; // AAIController

	UPROPERTY(Category = "Rotation", BlueprintReadOnly)
	TSet<UObject*> ControlRotationUpdateBlockSet;

	UPROPERTY()
	bool bPrevForceControlRotationOnPawnCallUseYawValue;

	UPROPERTY()
	bool bPrevForceControlRotationOnPawnCallOrientValue;
//~ End Rotation
};
