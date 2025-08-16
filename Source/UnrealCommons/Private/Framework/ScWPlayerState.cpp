// Scientific Ways

#include "Framework/ScWPlayerState.h"

#include "Framework/ScWLevelScriptActor.h"

#include "Gameplay/Progression/ScWProgressionTaskData.h"

AScWPlayerState::AScWPlayerState()
{
	
}

//~ Begin Initialize
void AScWPlayerState::PostInitializeComponents() // AActor
{
	Super::PostInitializeComponents();

}

void AScWPlayerState::BeginPlay() // AActor
{
	Super::BeginPlay();

	OnPawnSet.AddDynamic(this, &ThisClass::HandlePawnSet);
	UpdateOwnedASCActorInfo();

	UWorld* World = GetWorld();
	ensureReturn(World);

	AScWLevelScriptActor* LevelScriptActor = Cast<AScWLevelScriptActor>(World->GetLevelScriptActor());
	ensureReturn(LevelScriptActor);

	OnProgressionTaskMeterValueChangedDelegate.AddDynamic(LevelScriptActor, &AScWLevelScriptActor::BP_OnPlayerProgressionTaskMeterValueChanged);
	OnProgressionTaskMeterCompletedDelegate.AddDynamic(LevelScriptActor, &AScWLevelScriptActor::BP_OnPlayerCompletedProgressionTaskMeter);
	OnProgressionTaskFullyCompletedDelegate.AddDynamic(LevelScriptActor, &AScWLevelScriptActor::BP_OnPlayerCompletedProgressionTask);
}

void AScWPlayerState::EndPlay(const EEndPlayReason::Type InReason) // AActor
{
	

	Super::EndPlay(InReason);
}
//~ End Initialize

//~ Begin Pawn
void AScWPlayerState::HandlePawnSet(APlayerState* InPlayer, APawn* InNewPawn, APawn* InOldPawn)
{
	UpdateOwnedASCActorInfo();
}
//~ End Pawn

//~ Begin Ability System
bool AScWPlayerState::OwnsAbilitySystemComponent() const
{
	return GetOwnedAbilitySystemComponent() != nullptr;
}

UAbilitySystemComponent* AScWPlayerState::GetOwnedAbilitySystemComponent() const
{
	return FindComponentByClass<UAbilitySystemComponent>();
}

UAbilitySystemComponent* AScWPlayerState::GetAbilitySystemComponent() const // IAbilitySystemInterface
{
	if (UAbilitySystemComponent* OwnedASC = GetOwnedAbilitySystemComponent())
	{
		return OwnedASC;
	}
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPawn());
}

void AScWPlayerState::UpdateOwnedASCActorInfo()
{
	if (UAbilitySystemComponent* OwnedASC = GetOwnedAbilitySystemComponent())
	{
		if (APawn* CurrentPawn = GetPawn())
		{
			OwnedASC->SetAvatarActor(CurrentPawn);
		}
		else
		{
			OwnedASC->SetAvatarActor(this);
		}
	}
}
//~ End Ability System

//~ Begin Progression Tasks
const FScWProgressionTask_ProgressData& AScWPlayerState::GetProgressionTaskProgressData(UScWProgressionTaskData* InTaskData) const
{
	if (const FScWProgressionTask_ProgressData* OutData = ProgressionTasksProgressDataMap.Find(InTaskData))
	{
		return *OutData;
	}
	return FScWProgressionTask_ProgressData::Invalid;
}

int32 AScWPlayerState::GetProgressionTaskProgressMeterValue(UScWProgressionTaskData* InTaskData, const FName& InMeterName, const bool bInFailIfTaskWasMarkedAsCompleted) const
{
	ensureReturn(InTaskData, false);

	if (!HasProgressionTask(InTaskData, bInFailIfTaskWasMarkedAsCompleted))
	{
		return INDEX_NONE;
	}
	if (WasProgressionTaskCompleted(InTaskData))
	{
		return InTaskData->GetMeterTargetValue(InMeterName);
	}
	const FScWProgressionTask_ProgressData* SampleProgressData = ProgressionTasksProgressDataMap.Find(InTaskData);
	ensureReturn(SampleProgressData, INDEX_NONE);

	return SampleProgressData->MetersProgressMap.FindRef(InMeterName, 0);
}

bool AScWPlayerState::HasProgressionTask(UScWProgressionTaskData* InTaskData, const bool bInFailIfWasMarkedAsCompleted) const
{
	ensure((!ProgressionTasksProgressDataMap.Contains(InTaskData) && !CompletedProgressionTasksSet.Contains(InTaskData))
		|| (ProgressionTasksProgressDataMap.Contains(InTaskData) != CompletedProgressionTasksSet.Contains(InTaskData)));

	if (bInFailIfWasMarkedAsCompleted && CompletedProgressionTasksSet.Contains(InTaskData))
	{
		return false;
	}
	return ProgressionTasksProgressDataMap.Contains(InTaskData) || CompletedProgressionTasksSet.Contains(InTaskData);
}

bool AScWPlayerState::IsProgressionTaskMeterCompleted(UScWProgressionTaskData* InTaskData, const FName& InMeterName, const bool bInFailIfTaskWasMarkedAsCompleted) const
{
	if (!HasProgressionTask(InTaskData, bInFailIfTaskWasMarkedAsCompleted))
	{
		return false;
	}
	ensureReturn(InTaskData, false);

	const FScWProgressionTask_ProgressData* SampleProgressData = ProgressionTasksProgressDataMap.Find(InTaskData);
	ensureReturn(SampleProgressData, false);

	return SampleProgressData->MetersProgressMap.Contains(InMeterName) && (SampleProgressData->MetersProgressMap[InMeterName] == InTaskData->GetMeterTargetValue(InMeterName));
}

bool AScWPlayerState::IsProgressionTaskMetersFullyCompleted(UScWProgressionTaskData* InTaskData, const bool bInFailIfTaskWasMarkedAsCompleted) const
{
	if (!HasProgressionTask(InTaskData, bInFailIfTaskWasMarkedAsCompleted))
	{
		return false;
	}
	ensureReturn(InTaskData, false);
	for (const auto& SampleKeyValue : InTaskData->MetersMap)
	{
		if (IsProgressionTaskMeterCompleted(InTaskData, SampleKeyValue.Key, bInFailIfTaskWasMarkedAsCompleted))
		{
			// Good
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool AScWPlayerState::AddNewProgressionTask(UScWProgressionTaskData* InTaskData, FScWProgressionTask_ProgressData InDefaultProgressData, const bool bInFailIfWasAddedBefore)
{
	if (bInFailIfWasAddedBefore)
	{
		if (ProgressionTasksProgressDataMap.Contains(InTaskData) || CompletedProgressionTasksSet.Contains(InTaskData))
		{
			return false;
		}
	}
	ProgressionTasksProgressDataMap.Add(InTaskData, InDefaultProgressData);
	OnProgressionTaskAddedDelegate.Broadcast(this, InTaskData);

	if (CompletedProgressionTasksSet.Contains(InTaskData))
	{
		OnPreviouslyCompletedProgressionTaskAddedDelegate.Broadcast(this, InTaskData);
		CompletedProgressionTasksSet.Remove(InTaskData);
	}
	ensure(ProgressionTasksProgressDataMap.Contains(InTaskData) != CompletedProgressionTasksSet.Contains(InTaskData));
	return true;
}

bool AScWPlayerState::AddProgressionTasksMeterValue(const FName& InMeterName, const int32 InValue, UScWProgressionTaskData* InOptionalSpecificTaskData)
{
	ensureReturn(InValue != 0, false);

	if (InOptionalSpecificTaskData)
	{
		return HandleAddProgressionTaskMeterValue(InOptionalSpecificTaskData, InMeterName, InValue);
	}
	else
	{
		bool bAnyAdded = false;

		TArray<UScWProgressionTaskData*> ProgressionTaskDataArray;
		ProgressionTasksProgressDataMap.GenerateKeyArray(ProgressionTaskDataArray);

		for (UScWProgressionTaskData* SampleProgressionTaskData : ProgressionTaskDataArray)
		{
			bAnyAdded |= HandleAddProgressionTaskMeterValue(SampleProgressionTaskData, InMeterName, InValue);
		}
		return bAnyAdded;
	}
}

bool AScWPlayerState::HandleAddProgressionTaskMeterValue(UScWProgressionTaskData* InTaskData, const FName& InMeterName, const int32 InValue)
{
	ensureReturn(InTaskData, false);

	if (InTaskData->HasMeter(InMeterName) && HasProgressionTask(InTaskData, true))
	{
		if (FScWProgressionTask_ProgressData* SampleProgressData = ProgressionTasksProgressDataMap.Find(InTaskData))
		{
			int32& SampleMeterValue = SampleProgressData->MetersProgressMap.FindOrAdd(InMeterName);
			SampleMeterValue = FMath::Clamp(SampleMeterValue + InValue, 0, InTaskData->GetMeterTargetValue(InMeterName));

			OnProgressionTaskMeterValueChangedDelegate.Broadcast(this, InTaskData, InMeterName, SampleMeterValue);

			if (IsProgressionTaskMeterCompleted(InTaskData, InMeterName, true))
			{
				OnProgressionTaskMeterCompletedDelegate.Broadcast(this, InTaskData, InMeterName);
			}
			if (IsProgressionTaskMetersFullyCompleted(InTaskData, true))
			{
				ForceCompleteProgressionTask(InTaskData);
			}
			return true;
		}
	}
	return false;
}

bool AScWPlayerState::ForceCompleteProgressionTask(UScWProgressionTaskData* InTaskData)
{
	if (HasProgressionTask(InTaskData, true))
	{
		ProgressionTasksProgressDataMap.Remove(InTaskData);
		CompletedProgressionTasksSet.Add(InTaskData);
		OnProgressionTaskFullyCompletedDelegate.Broadcast(this, InTaskData);
		return true;
	}
	return false;
}
//~ End Progression Tasks

//~ Begin Team
void AScWPlayerState::SetGenericTeamId(const FGenericTeamId& InNewTeamId) // IGenericTeamAgentInterface
{
	TeamId = InNewTeamId;
}
//~ End Team
