// Scientific Ways

#include "Gameplay/Tasks/ScWAT_WaitDelay.h"

//~ Begin Initialize
UScWAT_WaitDelay* UScWAT_WaitDelay::WaitDelayOrFinishImmediately(UGameplayAbility* InOwningAbility, float InTime)
{
	UScWAT_WaitDelay* OutTaskObject = NewAbilityTask<UScWAT_WaitDelay>(InOwningAbility);
	OutTaskObject->bCanFinishImmediately = true;
	OutTaskObject->Time = InTime;
	OutTaskObject->TimeStarted = 0.0f;
	OutTaskObject->TimerHandle = FTimerHandle();
	return OutTaskObject;
}

UScWAT_WaitDelay* UScWAT_WaitDelay::WaitDelayOrFinishNextTick(UGameplayAbility* InOwningAbility, float InTime)
{
	UScWAT_WaitDelay* OutTaskObject = NewAbilityTask<UScWAT_WaitDelay>(InOwningAbility);
	OutTaskObject->bCanFinishImmediately = false;
	OutTaskObject->Time = InTime;
	OutTaskObject->TimeStarted = 0.0f;
	OutTaskObject->TimerHandle = FTimerHandle();
	return OutTaskObject;
}

void UScWAT_WaitDelay::Activate() // UGameplayTask
{
	if (Time > 0.0f)
	{
		if (UWorld* World = GetWorld())
		{
			TimeStarted = World->GetTimeSeconds();
			World->GetTimerManager().SetTimer(TimerHandle, this, &UScWAT_WaitDelay::OnTimeFinish, Time, false);
		}
		else
		{
			ensure(false);
			EndTask();
		}
	}
	else
	{
		if (bCanFinishImmediately)
		{
			OnTimeFinish();
		}
		else
		{
			if (UWorld* World = GetWorld())
			{
				TimeStarted = World->GetTimeSeconds();
				TimerHandle = World->GetTimerManager().SetTimerForNextTick(this, &UScWAT_WaitDelay::OnTimeFinish);
			}
			else
			{
				ensure(false);
				EndTask();
			}
		}
	}
}

void UScWAT_WaitDelay::OnDestroy(bool bInAbilityIsEnding)
{
	UWorld* World = GetWorld();
	if (World && TimerHandle.IsValid())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Debug
FString UScWAT_WaitDelay::GetDebugString() const // UGameplayTask
{
	if (UWorld* World = GetWorld())
	{
		const float TimeLeft = Time - World->TimeSince(TimeStarted);
		return FString::Printf(TEXT("WaitDelay. Time: %.2f. TimeLeft: %.2f"), Time, TimeLeft);
	}
	else
	{
		return FString::Printf(TEXT("WaitDelay. Time: %.2f. Time Started: %.2f"), Time, TimeStarted);
	}
}
//~ End Debug

//~ Begin Callbacks
void UScWAT_WaitDelay::OnTimeFinish()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnFinish.Broadcast();
	}
	EndTask();
}
//~ End Callbacks
