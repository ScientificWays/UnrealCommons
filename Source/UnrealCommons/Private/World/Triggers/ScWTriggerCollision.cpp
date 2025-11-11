// Scientific Ways

#include "World/Triggers/ScWTriggerCollision.h"

#include "Gameplay/ScWGameplayFunctionLibrary.h"

AScWTriggerCollision::AScWTriggerCollision()
{
	PrimaryActorTick.bCanEverTick = false;

	bNetLoadOnClient = false;
	bReplicates = false;
	SetNetUpdateFrequency(1.0f);
	SetMinNetUpdateFrequency(0.1f);
	SetReplicatingMovement(false);

	bGenerateOverlapEventsDuringLevelStreaming = true;
	bEnableAutoLODGeneration = false;

#if WITH_EDITORONLY_DATA
	bIsSpatiallyLoaded = false;
#endif // WITH_EDITORONLY_DATA

	bIsEnabled = true;

	bDestroyAfterBeginOverlap = false;
	bDestroyAfterEndOverlap = false;

	TriggerCooldown = 0.0f;
}

//~ Begin Initialize
void AScWTriggerCollision::PostInitializeComponents() // AActor
{
	Super::PostInitializeComponents();


}

void AScWTriggerCollision::OnConstruction(const FTransform& InTransform) // AActor
{
	Super::OnConstruction(InTransform);

	HandleIsEnabledChanged();
}

void AScWTriggerCollision::BeginPlay() // AActor
{
	Super::BeginPlay();

	HandleIsEnabledChanged();
}
//~ End Initialize

//~ Begin Overlaps
bool AScWTriggerCollision::IsAllActorsOverlapTrigger(TArray<AActor*> InActorArray) const
{
	for (AActor* SampleActor : InActorArray)
	{
		ensureContinue(SampleActor);

		if (!IsOverlappingActor(SampleActor))
		{
			return false;
		}
	}
	return true;
}

bool AScWTriggerCollision::IsAnyActorOverlapsTrigger(TArray<AActor*> InActorArray) const
{
	for (AActor* SampleActor : InActorArray)
	{
		ensureContinue(SampleActor);

		if (IsOverlappingActor(SampleActor))
		{
			return true;
		}
	}
	return false;
}

void AScWTriggerCollision::NotifyActorBeginOverlap(AActor* InOtherActor) // AActor
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
		ForEachComponent(true, [](UActorComponent* InComponent)
		{
			if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(InComponent))
			{
				PrimitiveComponent->SetGenerateOverlapEvents(false);
			}
		});
		FTimerManager& TimerManager = GetWorldTimerManager();
		ensure(!CooldownTimerHandle.IsValid());

		TimerManager.ClearTimer(CooldownTimerHandle);
		TimerManager.SetTimer(CooldownTimerHandle, this, &AScWTriggerCollision::HandleCooldownTimerExpired, TriggerCooldown, false);
	}
}

void AScWTriggerCollision::NotifyActorEndOverlap(AActor* InOtherActor) // AActor
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
void AScWTriggerCollision::SetEnabled(const bool bInNewEnabled)
{
	if (bIsEnabled == bInNewEnabled)
	{
		return;
	}
	bIsEnabled = bInNewEnabled;
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

void AScWTriggerCollision::HandleIsEnabledChanged()
{
	bool bGenerateOverlapEvents = bIsEnabled;

	ForEachComponent(true, [bGenerateOverlapEvents](UActorComponent* InComponent)
	{
		if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(InComponent))
		{
			PrimitiveComponent->SetGenerateOverlapEvents(bGenerateOverlapEvents);
		}
	});
}
//~ End Toggle

//~ Begin Cooldown
bool AScWTriggerCollision::IsOnCooldown() const
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	return TimerManager.TimerExists(CooldownTimerHandle);
}

void AScWTriggerCollision::HandleCooldownTimerExpired()
{
	if (IsEnabled())
	{
		bool bGenerateOverlapEvents = bIsEnabled;

		ForEachComponent(true, [bGenerateOverlapEvents](UActorComponent* InComponent)
		{
			if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(InComponent))
			{
				PrimitiveComponent->SetGenerateOverlapEvents(bGenerateOverlapEvents);
			}
		});
	}
	OnCooldownExpired.Broadcast();
}
//~ End Cooldown

//~ Begin Teams
bool AScWTriggerCollision::CheckAllTeamMembersOverlap() const
{
	ensureReturn(!TargetTeamSet.IsEmpty(), false);

	TArray<AActor*> TargetTeamsActorArray = UScWGameplayFunctionLibrary::GetAllActorsOfAnyTeam(this, TargetTeamSet);
	return IsAllActorsOverlapTrigger(TargetTeamsActorArray);
}

bool AScWTriggerCollision::CheckNoneTeamMembersOverlap() const
{
	ensureReturn(!TargetTeamSet.IsEmpty(), false);

	TArray<AActor*> TargetTeamsActorArray = UScWGameplayFunctionLibrary::GetAllActorsOfAnyTeam(this, TargetTeamSet);
	return !IsAnyActorOverlapsTrigger(TargetTeamsActorArray);
}
//~ End Teams
