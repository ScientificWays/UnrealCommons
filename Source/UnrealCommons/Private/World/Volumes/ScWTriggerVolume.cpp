// Scientific Ways

#include "World/Volumes/ScWTriggerVolume.h"

#include "Gameplay/ScWGameplayFunctionLibrary.h"

AScWTriggerVolume::AScWTriggerVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	GetBrushComponent()->bReceivesDecals = false;
	GetBrushComponent()->SetCollisionProfileName(TEXT("Trigger"));

	bNetLoadOnClient = false;
	bReplicates = false;
	SetNetUpdateFrequency(1.0f);
	SetMinNetUpdateFrequency(0.1f);
	SetReplicatingMovement(false);

	bEnableAutoLODGeneration = false;

#if WITH_EDITORONLY_DATA
	bIsSpatiallyLoaded = false;

	GetBrushComponent()->bEnableAutoLODGeneration = false;

	bDisplayShadedVolume = true;
	ShadedVolumeOpacityValue = 0.1f;
#endif // WITH_EDITORONLY_DATA

	bIsEnabled = true;

	bDestroyAfterBeginOverlap = false;
	bDestroyAfterEndOverlap = false;

	TriggerCooldown = 0.0f;
}

//~ Begin Initialize
void AScWTriggerVolume::PostInitializeComponents() // AActor
{
	Super::PostInitializeComponents();


}

void AScWTriggerVolume::OnConstruction(const FTransform& InTransform) // AActor
{
	Super::OnConstruction(InTransform);

	HandleIsEnabledChanged();
}

void AScWTriggerVolume::BeginPlay() // AActor
{
	Super::BeginPlay();


}
//~ End Initialize

//~ Begin Overlaps
bool AScWTriggerVolume::IsActorInsideVolume(const AVolume* InVolume, AActor* InActor)
{
	ensureReturn(InVolume, false);
	ensureReturn(InActor, false);
	return InVolume->EncompassesPoint(InActor->GetActorLocation());
}

bool AScWTriggerVolume::IsAllActorsInsideVolume(const AVolume* InVolume, TArray<AActor*> InActorArray)
{
	ensureReturn(InVolume, false);
	
	for (AActor* SampleActor : InActorArray)
	{
		if (!InVolume->EncompassesPoint(SampleActor->GetActorLocation()))
		{
			return false;
		}
	}
	return true;
}

bool AScWTriggerVolume::IsAnyActorInsideVolume(const AVolume* InVolume, TArray<AActor*> InActorArray)
{
	ensureReturn(InVolume, false);
	
	for (AActor* SampleActor : InActorArray)
	{
		ensureContinue(SampleActor);

		if (InVolume->EncompassesPoint(SampleActor->GetActorLocation()))
		{
			return true;
		}
	}
	return false;
}

bool AScWTriggerVolume::IsAllActorsOverlapVolume(const AVolume* InVolume, TArray<AActor*> InActorArray)
{
	ensureReturn(InVolume, false);
	
	for (AActor* SampleActor : InActorArray)
	{
		ensureContinue(SampleActor);

		if (!InVolume->IsOverlappingActor(SampleActor))
		{
			return false;
		}
	}
	return true;
}

bool AScWTriggerVolume::IsAnyActorOverlapsVolume(const AVolume* InVolume, TArray<AActor*> InActorArray)
{
	ensureReturn(InVolume, false);
	
	for (AActor* SampleActor : InActorArray)
	{
		ensureContinue(SampleActor);

		if (InVolume->IsOverlappingActor(SampleActor))
		{
			return true;
		}
	}
	return false;
}

void AScWTriggerVolume::NotifyActorBeginOverlap(AActor* InOtherActor) // AActor
{
	if (IsOnCooldown())
	{
		return;
	}
	Super::NotifyActorBeginOverlap(InOtherActor);

	if (!TargetTeamSet.IsEmpty())
	{
		const FName& OtherTeamName = UScWGameplayFunctionLibrary::GetActorTeamName(InOtherActor);
		if (TargetTeamSet.Contains(OtherTeamName))
		{
			OnTeamMemberBeginOverlap.Broadcast(this, InOtherActor);
			if (bCheckForAllTeamMembers && CheckAllTeamMembersOverlap())
			{
				OnAllTeamMembersOverlap.Broadcast(this);
			}
			if (bDestroyAfterTeamMemberBeginOverlap)
			{
				Destroy();
			}
		}
	}
	if (bDestroyAfterBeginOverlap)
	{
		Destroy();
	}
	else if (TriggerCooldown > 0.0f)
	{
		ensureReturn(GetBrushComponent());
		GetBrushComponent()->SetGenerateOverlapEvents(false);

		FTimerManager& TimerManager = GetWorldTimerManager();
		ensure(!CooldownTimerHandle.IsValid());

		TimerManager.ClearTimer(CooldownTimerHandle);
		TimerManager.SetTimer(CooldownTimerHandle, this, &AScWTriggerVolume::HandleCooldownTimerExpired, TriggerCooldown, false);
	}
}

void AScWTriggerVolume::NotifyActorEndOverlap(AActor* InOtherActor) // AActor
{
	Super::NotifyActorEndOverlap(InOtherActor);

	if (!TargetTeamSet.IsEmpty())
	{
		const FName& OtherTeamName = UScWGameplayFunctionLibrary::GetActorTeamName(InOtherActor);
		if (TargetTeamSet.Contains(OtherTeamName))
		{
			OnTeamMemberEndOverlap.Broadcast(this, InOtherActor);
			if (bCheckForAllTeamMembers && CheckNoneTeamMembersOverlap())
			{
				OnNoneTeamMembersOverlap.Broadcast(this);
			}
			if (bDestroyAfterTeamMemberEndOverlap)
			{
				Destroy();
			}
		}
	}
	if (bDestroyAfterEndOverlap)
	{
		Destroy();
	}
}
//~ End Overlaps

//~ Begin Toggle
void AScWTriggerVolume::SetEnabled(const bool bNewEnabled)
{
	if (bIsEnabled == bNewEnabled)
	{
		return;
	}
	bIsEnabled = bNewEnabled;
	HandleIsEnabledChanged();

	if (bIsEnabled)
	{
		OnEnabled.Broadcast();
	}
	else
	{
		OnDisabled.Broadcast();
	}
}

void AScWTriggerVolume::HandleIsEnabledChanged()
{
	ensureReturn(GetBrushComponent());
	GetBrushComponent()->SetGenerateOverlapEvents(bIsEnabled);
}
//~ End Toggle

//~ Begin Cooldown
bool AScWTriggerVolume::IsOnCooldown() const
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	return TimerManager.TimerExists(CooldownTimerHandle);
}

void AScWTriggerVolume::HandleCooldownTimerExpired()
{
	if (IsEnabled())
	{
		ensureReturn(GetBrushComponent());
		GetBrushComponent()->SetGenerateOverlapEvents(true);
	}
	OnCooldownExpired.Broadcast();
}
//~ End Cooldown

//~ Begin Teams
bool AScWTriggerVolume::CheckAllTeamMembersOverlap() const
{
	ensureReturn(!TargetTeamSet.IsEmpty(), false);

	TArray<AActor*> TargetTeamsActorArray = UScWGameplayFunctionLibrary::GetAllActorsOfAnyTeam(this, TargetTeamSet);
	return AScWTriggerVolume::IsAllActorsOverlapVolume(this, TargetTeamsActorArray);
}

bool AScWTriggerVolume::CheckNoneTeamMembersOverlap() const
{
	ensureReturn(!TargetTeamSet.IsEmpty(), false);

	TArray<AActor*> TargetTeamsActorArray = UScWGameplayFunctionLibrary::GetAllActorsOfAnyTeam(this, TargetTeamSet);
	return !AScWTriggerVolume::IsAnyActorOverlapsVolume(this, TargetTeamsActorArray);
}
//~ End Teams
