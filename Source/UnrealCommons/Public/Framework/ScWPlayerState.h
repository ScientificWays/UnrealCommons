// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Progression/ScWTypes_Progression.h"

#include "ScWPlayerState.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Player State"))
class UNREALCOMMONS_API AScWPlayerState : public APlayerState, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	AScWPlayerState();

//~ Begin Initialize
protected:
	virtual void PostInitializeComponents() override; // AActor
	virtual void BeginPlay() override; // AActor
	virtual void EndPlay(const EEndPlayReason::Type InReason) override; // AActor
//~ End Initialize
	
//~ Begin AbilitySystem
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface
//~ End AbilitySystem

//~ Begin Progression Tasks
public:

	UFUNCTION(Category = "Progression Tasks | Getters", BlueprintCallable)
	const TMap<class UScWProgressionTaskData*, FScWProgressionTask_ProgressData>& GetProgressionTasksProgressDataMap() const { return ProgressionTasksProgressDataMap; }
	
	UFUNCTION(Category = "Progression Tasks | Getters", BlueprintCallable)
	const FScWProgressionTask_ProgressData& GetProgressionTaskProgressData(class UScWProgressionTaskData* InTaskData) const;

	UFUNCTION(Category = "Progression Tasks | Getters", BlueprintCallable, meta = (AutoCreateRefTerm = "InMeterName"))
	int32 GetProgressionTaskProgressMeterValue(class UScWProgressionTaskData* InTaskData, const FName& InMeterName, const bool bInFailIfTaskWasMarkedAsCompleted) const;

	UFUNCTION(Category = "Progression Tasks | Getters", BlueprintCallable)
	bool HasProgressionTask(class UScWProgressionTaskData* InTaskData, const bool bInFailIfWasMarkedAsCompleted) const;

	UFUNCTION(Category = "Progression Tasks | Getters", BlueprintCallable, meta = (AutoCreateRefTerm = "InMeterName"))
	bool IsProgressionTaskMeterCompleted(class UScWProgressionTaskData* InTaskData, const FName& InMeterName, const bool bInFailIfTaskWasMarkedAsCompleted) const;

	UFUNCTION(Category = "Progression Tasks | Getters", BlueprintCallable)
	bool IsProgressionTaskMetersFullyCompleted(class UScWProgressionTaskData* InTaskData, const bool bInFailIfTaskWasMarkedAsCompleted) const;

	UFUNCTION(Category = "Progression Tasks | Setters", BlueprintCallable, meta = (KeyWords = "AddTask"))
	bool AddNewProgressionTask(class UScWProgressionTaskData* InTaskData, FScWProgressionTask_ProgressData InDefaultProgressData = FScWProgressionTask_ProgressData(), const bool bInFailIfWasAddedBefore = true);

	UFUNCTION(Category = "Progression Tasks | Setters", BlueprintCallable, meta = (AutoCreateRefTerm = "InMeterName", InValue = "1", KeyWords = "AddTask", AdvancedDisplay = "InOptionalSpecificTaskData"))
	bool AddProgressionTasksMeterValue(const FName& InMeterName, const int32 InValue, class UScWProgressionTaskData* InOptionalSpecificTaskData = nullptr);

	UFUNCTION(Category = "Progression Tasks | Setters", BlueprintCallable)
	bool ForceCompleteProgressionTask(class UScWProgressionTaskData* InTaskData);

	UPROPERTY(Category = "Progression Tasks", BlueprintAssignable)
	FScWProgressionTaskEventSignature OnProgressionTaskAddedDelegate;

	UPROPERTY(Category = "Progression Tasks", BlueprintAssignable)
	FScWProgressionTaskEventSignature OnPreviouslyCompletedProgressionTaskAddedDelegate;

	UPROPERTY(Category = "Progression Tasks", BlueprintAssignable)
	FScWProgressionTaskMeterValueEventSignature OnProgressionTaskMeterValueChangedDelegate;

	UPROPERTY(Category = "Progression Tasks", BlueprintAssignable)
	FScWProgressionTaskMeterEventSignature OnProgressionTaskMeterCompletedDelegate;

	UPROPERTY(Category = "Progression Tasks", BlueprintAssignable)
	FScWProgressionTaskEventSignature OnProgressionTaskFullyCompletedDelegate;

	UFUNCTION(Category = "Progression Tasks | Completed", BlueprintCallable)
	const TSet<class UScWProgressionTaskData*>& GetCompletedProgressionTasksSet() const { return CompletedProgressionTasksSet; }

	UFUNCTION(Category = "Progression Tasks | Completed", BlueprintCallable, meta = (KeyWords = "WasTaskCompleted, IsProgressionTaskCompleted, IsTaskCompleted"))
	bool WasProgressionTaskCompleted(class UScWProgressionTaskData* InTaskData) const { return CompletedProgressionTasksSet.Contains(InTaskData); }

protected:
	bool HandleAddProgressionTaskMeterValue(class UScWProgressionTaskData* InTaskData, const FName& InMeterName, const int32 InValue);

	UPROPERTY(Transient)
	TMap<class UScWProgressionTaskData*, FScWProgressionTask_ProgressData> ProgressionTasksProgressDataMap;

	UPROPERTY(Transient)
	TSet<class UScWProgressionTaskData*> CompletedProgressionTasksSet;
//~ End Progression Tasks
	
//~ Begin Team
public:
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; } // IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& InNewTeamID) override; // IGenericTeamAgentInterface
private:
	FGenericTeamId TeamId;
//~ End Team
};
