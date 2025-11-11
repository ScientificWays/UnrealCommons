// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_CommonDelegates.h"

#include "ScWTriggerVolume.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTriggerVolumeSignature, class AScWTriggerVolume*, InTriggerVolume);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTriggerVolumeActorSignature, class AScWTriggerVolume*, InTriggerVolume, AActor*, InOtherActor);

/**
* 
*/
UCLASS(Blueprintable, meta = (DisplayName = "[ScW] Trigger Volume (Base)"))
class UNREALCOMMONS_API AScWTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()

public:	

	AScWTriggerVolume();

//~ Begin Initialize
protected:
	virtual void PostInitializeComponents() override; // AActor
	virtual void OnConstruction(const FTransform& InTransform) override; // AActor
	virtual void BeginPlay() override; // AActor
//~ End Initialize
	
//~ Begin Overlaps
public:

	UFUNCTION(Category = "Overlaps", BlueprintCallable, BlueprintPure)
	static bool IsActorInsideVolume(const AVolume* InVolume, AActor* InActor);

	UFUNCTION(Category = "Overlaps", BlueprintCallable, BlueprintPure, meta = (KeyWords = "AreAllActorsInsideVolume"))
	static bool IsAllActorsInsideVolume(const AVolume* InVolume, TArray<AActor*> InActorArray);

	UFUNCTION(Category = "Overlaps", BlueprintCallable, BlueprintPure)
	static bool IsAnyActorInsideVolume(const AVolume* InVolume, TArray<AActor*> InActorArray);

	UFUNCTION(Category = "Overlaps", BlueprintCallable, BlueprintPure)
	static bool IsAllActorsOverlapVolume(const AVolume* InVolume, TArray<AActor*> InActorArray);

	UFUNCTION(Category = "Overlaps", BlueprintCallable, BlueprintPure)
	static bool IsAnyActorOverlapsVolume(const AVolume* InVolume, TArray<AActor*> InActorArray);

	UPROPERTY(Category = "Overlap", EditAnywhere)
	bool bDestroyAfterBeginOverlap;

	UPROPERTY(Category = "Overlap", EditAnywhere)
	bool bDestroyAfterEndOverlap;

protected:
	virtual void NotifyActorBeginOverlap(AActor* InOtherActor) override; // AActor
	virtual void NotifyActorEndOverlap(AActor* InOtherActor) override; // AActor
//~ End Overlaps

//~ Begin Toggle
public:

	UFUNCTION(Category = "Toggle", BlueprintCallable)
	bool IsEnabled() const { return bIsEnabled; }
	
	UFUNCTION(Category = "Toggle", BlueprintCallable, meta = (KeyWords = "GenerateOverlapEvents"))
	void SetEnabled(const bool bNewEnabled);

	UPROPERTY(Category = "Toggle", BlueprintAssignable)
	FDefaultEventSignature OnEnabled;
	
	UPROPERTY(Category = "Toggle", BlueprintAssignable)
	FDefaultEventSignature OnDisabled;

protected:
	virtual void HandleIsEnabledChanged();

	UPROPERTY(Category = "Toggle", EditAnywhere, meta = (DisplayName = "Enabled (Generate overlap events)"))
	bool bIsEnabled;
//~ End Toggle

//~ Begin Cooldown
public:
	
	UFUNCTION(Category = "Cooldown", BlueprintCallable)
	bool IsOnCooldown() const;
	
	UPROPERTY(Category = "Cooldown", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bDestroyAfterBeginOverlap"))
	float TriggerCooldown;
	
	UPROPERTY(Category = "Cooldown", BlueprintAssignable)
	FDefaultEventSignature OnCooldownExpired;

protected:
	void HandleCooldownTimerExpired();
	
	UPROPERTY(Transient)
	FTimerHandle CooldownTimerHandle;
//~ End Cooldown
	
//~ Begin Teams
public:

	UFUNCTION(Category = "Teams", BlueprintCallable)
	bool CheckAllTeamMembersOverlap() const;
	
	UFUNCTION(Category = "Teams", BlueprintCallable)
	bool CheckNoneTeamMembersOverlap() const;
	
	UPROPERTY(Category = "Teams", EditAnywhere)
	TSet<FName> TargetTeamSet;

	UPROPERTY(Category = "Teams", EditAnywhere)
	TSubclassOf<AActor> TargetTeamFilterActorClass;

	UPROPERTY(Category = "Teams", EditAnywhere, meta = (EditCondition = "!TargetTeamSet.IsEmpty()"))
	bool bCheckForAllTeamMembers;

	UPROPERTY(Category = "Teams", EditAnywhere, meta = (EditCondition = "!TargetTeamSet.IsEmpty()"))
	bool bDestroyAfterTeamMemberBeginOverlap;

	UPROPERTY(Category = "Teams", EditAnywhere, meta = (EditCondition = "!TargetTeamSet.IsEmpty()"))
	bool bDestroyAfterTeamMemberEndOverlap;

	UPROPERTY(Category = "Teams", BlueprintAssignable)
	FTriggerVolumeActorSignature OnTeamMemberBeginOverlap;
	
	UPROPERTY(Category = "Teams", BlueprintAssignable)
	FTriggerVolumeActorSignature OnTeamMemberEndOverlap;
	
	UPROPERTY(Category = "Teams", BlueprintAssignable)
	FTriggerVolumeSignature OnAllTeamMembersOverlap;
	
	UPROPERTY(Category = "Teams", BlueprintAssignable)
	FTriggerVolumeSignature OnNoneTeamMembersOverlap;
//~ End Teams
};
