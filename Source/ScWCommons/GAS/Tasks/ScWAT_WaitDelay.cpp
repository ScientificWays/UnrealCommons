// G.o.o.P Studios

#include "GAS/Tasks/ATAAT_WaitDelay.h"

UATAAT_WaitDelay::UATAAT_WaitDelay()
{
	TimerHandle = FTimerHandle();

	Time = 0.0f;
	TimeStarted = 0.0f;
}

UATAAT_WaitDelay* UATAAT_WaitDelay::WaitDelayOrFinishImmediately(UGameplayAbility* InOwningAbility, float InTime)
{
	UATAAT_WaitDelay* OutTaskObject = NewAbilityTask<UATAAT_WaitDelay>(InOwningAbility);
	OutTaskObject->Time = InTime;
	return OutTaskObject;
}

//~ Begin Debug
FString UATAAT_WaitDelay::GetDebugString() const // UGameplayTask
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

//~ Begin Initialize
void UATAAT_WaitDelay::Activate() // UGameplayTask
{
	if (Time > 0.0f)
	{
		UWorld* World = GetWorld();
		TimeStarted = World->GetTimeSeconds();
		World->GetTimerManager().SetTimer(TimerHandle, this, &UATAAT_WaitDelay::OnTimeFinish, Time, false);
	}
	else
	{
		OnTimeFinish();
	}
}

void UATAAT_WaitDelay::OnDestroy(bool bInAbilityIsEnding)
{
	UWorld* World = GetWorld();
	if (World && TimerHandle.IsValid())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Callbacks
void UATAAT_WaitDelay::OnTimeFinish()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnFinish.Broadcast();
	}
	EndTask();
}
//~ End Callbacks
