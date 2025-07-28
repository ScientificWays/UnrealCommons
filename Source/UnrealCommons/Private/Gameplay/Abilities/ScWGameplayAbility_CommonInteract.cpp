// Scientific Ways

#include "Gameplay/Abilities/ScWGameplayAbility_CommonInteract.h"

#include "AI/ScWTypes_AI.h"

#include "Animations/ScWAnimationsFunctionLibrary.h"

#include "Gameplay/ScWGameplayTags.h"
#include "Gameplay/ScWTypes_Gameplay.h"
#include "Gameplay/Tasks/ScWAT_WaitDelay.h"
#include "Gameplay/Characters/ScWCharacter.h"
#include "Gameplay/ScWGameplayFunctionLibrary.h"
#include "Gameplay/Interact/ScWInteractComponent.h"

UScWGameplayAbility_CommonInteract::UScWGameplayAbility_CommonInteract()
{
	{
		auto Tags = GetAssetTags();
		Tags.AddTag(FScWGameplayTags::Ability_Interact);
		SetAssetTags(Tags);
	}
	CancelAbilitiesWithTag.AddTag(FScWGameplayTags::Ability_CancelBy_InteractActivate);
	BlockAbilitiesWithTag.AddTag(FScWGameplayTags::Ability_CancelBy_InteractActivate);

	ActivationOwnedTags.AddTag(FScWGameplayTags::State_Interacting);

	CanInteractCheckCooldown = 0.1f;
	bLoopIfInputIsPressed = true;

	InteractTraceTypeQuery = TraceTypeQuery_Interact;
	InteractTraceDebugData = FScWTraceDebugData::None;
}

//~ Begin Ability
void UScWGameplayAbility_CommonInteract::NativeActivateAbility_Commited(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UGameplayAbility
{
	ensureCancelAbilityReturn(InActorInfo);
	ensureCancelAbilityReturn(InActorInfo->AvatarActor.IsValid());

	OwnerInteractComponent = InActorInfo->AvatarActor->FindComponentByClass<UScWInteractComponent>();

	BeginInteractSequence();
}

void UScWGameplayAbility_CommonInteract::BeginInteractSequence()
{
	bool bStarted = BP_TryStartInteract();

	if (bStarted)
	{
		COMMON_WAIT_DELAY_OR_FINISH_NEXT_TICK_TASK(InteractCheckCooldownTask, CanInteractCheckCooldown, OnCanInteractCheckCooldownFinished);
	}
	else
	{
		COMMON_WAIT_DELAY_OR_FINISH_NEXT_TICK_TASK(InteractCooldownTask, BP_GetInteractCooldown(), OnInteractCooldownFinished);
	}
}

void UScWGameplayAbility_CommonInteract::OnInteractCooldownFinished()
{
	if (bLoopIfInputIsPressed && IsAbilityInputPressed())
	{
		BeginInteractSequence();
	}
	else
	{
		BP_HandlePostInteract();
	}
}

void UScWGameplayAbility_CommonInteract::OnCanInteractCheckCooldownFinished()
{
	bool bFinished = BP_TryFinishInteract(false);

	if (bFinished)
	{
		BP_HandlePostInteract();
	}
	else
	{
		COMMON_WAIT_DELAY_OR_FINISH_NEXT_TICK_TASK(InteractCheckCooldownTask, CanInteractCheckCooldown, OnCanInteractCheckCooldownFinished);
	}
}

void UScWGameplayAbility_CommonInteract::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);

	BP_TryFinishInteract(true);
	TrySendAIMessageToOwner(FScWAIMessage::InteractEnded, !bInWasCancelled);
}
//~ End Ability

//~ Begin Interact
bool UScWGameplayAbility_CommonInteract::BP_TryStartInteract_Implementation()
{
	ensureReturn(OwnerInteractComponent, false);
	FVector TraceStart = OwnerInteractComponent->BP_GetInteractTraceStartLocation();
	FVector TraceEnd = TraceStart + OwnerInteractComponent->BP_GetInteractTraceDirection() * OwnerInteractComponent->BP_GetInteractTraceMaxDistance();

	TArray<FHitResult> TraceHitResults;
	FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerInteractComponent->GetOwner());
	UKismetSystemLibrary::SphereTraceMulti(this, TraceStart, TraceEnd, OwnerInteractComponent->BP_GetInteractTraceWidth(), InteractTraceTypeQuery, false, ActorsToIgnore, InteractTraceDebugData.DrawDebugType, TraceHitResults, true, InteractTraceDebugData.TraceColor, InteractTraceDebugData.TraceHitColor, InteractTraceDebugData.DrawTime);

	UWorld* World = GetWorld();
	ensureReturn(World, false);

	AGameStateBase* GameState = World->GetGameState();
	ensureReturn(World, false);

	float ServerWorldTimeSeconds = GameState->GetServerWorldTimeSeconds();

	float InteractTraceCheckRadius = OwnerInteractComponent->BP_GetInteractTraceCheckRadius();
	for (const FHitResult& SampleHitResult : TraceHitResults)
	{
		const FVector& SampleHitLocation = SampleHitResult.Location;
		UScWInteractComponent* SampleInteractTarget = UScWGameplayFunctionLibrary::FindInteractTargetInLocation(this, SampleHitLocation, InteractTraceCheckRadius, InteractTraceTypeQuery, InteractTraceDebugData, ActorsToIgnore);

		float InteractDuration = 0.0f;
		if (SampleInteractTarget->BP_CanReceiveInteractFrom(OwnerInteractComponent, InteractDuration))
		{
			FScWInteractData NewInteractData = { OwnerInteractComponent, SampleInteractTarget, ServerWorldTimeSeconds, ServerWorldTimeSeconds + InteractDuration };

			OwnerInteractComponent->SetInteractData(NewInteractData);
			SampleInteractTarget->SetInteractData(NewInteractData);

			OwnerInteractComponent->OnEmitInteractStart.Broadcast(SampleInteractTarget);
			SampleInteractTarget->OnReceiveInteractStart.Broadcast(OwnerInteractComponent);
			return true;
		}
		else
		{
			OwnerInteractComponent->OnEmitInteractFailed.Broadcast(SampleInteractTarget);
			SampleInteractTarget->OnReceiveInteractFailed.Broadcast(OwnerInteractComponent);
		}
	}
	return false;
}

bool UScWGameplayAbility_CommonInteract::BP_TryFinishInteract_Implementation(const bool bInWasCancelled)
{
	if (BP_ShouldFinishInteract(bInWasCancelled))
	{
		BP_HandleFinishInteract(bInWasCancelled);
		return true;
	}
	return false;
}

bool UScWGameplayAbility_CommonInteract::BP_ShouldFinishInteract_Implementation(const bool bInWasCancelled)
{
	ensureReturn(OwnerInteractComponent, true);
	const auto& CurrentInteractData = OwnerInteractComponent->GetInteractData();

	const float InteractMaxDistance = OwnerInteractComponent->BP_GetInteractTraceMaxDistance();

	UWorld* World = GetWorld();
	ensureReturn(World, true);

	AGameStateBase* GameState = World->GetGameState();
	ensureReturn(World, true);

	float ServerWorldTimeSeconds = GameState->GetServerWorldTimeSeconds();

	if (!IsValid(CurrentInteractData.Source) || !IsValid(CurrentInteractData.Target))
	{
		return true;
	}
	ensureReturn(OwnerInteractComponent->GetOwner(), true);
	const bool bForceCancel = bInWasCancelled || (OwnerInteractComponent->BP_GetInteractTraceStartLocation() - OwnerInteractComponent->GetOwner()->GetActorLocation()).SizeSquared() > FMath::Square(InteractMaxDistance);

	return bForceCancel || ServerWorldTimeSeconds >= CurrentInteractData.FinishTime;
}

void UScWGameplayAbility_CommonInteract::BP_HandleFinishInteract_Implementation(const bool bInWasCancelled)
{
	ensureReturn(OwnerInteractComponent);
	const auto& CurrentInteractData = OwnerInteractComponent->GetInteractData();

	if (UScWInteractComponent* TargetInteractComponent = CurrentInteractData.Target)
	{
		TargetInteractComponent->OnReceiveInteractFinish.Broadcast(bInWasCancelled);
		TargetInteractComponent->ResetInteractData();
	}
	OwnerInteractComponent->OnEmitInteractFinish.Broadcast(bInWasCancelled);
	OwnerInteractComponent->ResetInteractData();
}

void UScWGameplayAbility_CommonInteract::BP_HandlePostInteract_Implementation()
{
	K2_EndAbility();
}

float UScWGameplayAbility_CommonInteract::BP_GetInteractCooldown_Implementation() const
{
	return 0.2f;
}
//~ End Interact
