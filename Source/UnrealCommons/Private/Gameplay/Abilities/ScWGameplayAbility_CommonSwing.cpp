// Scientific Ways

#include "Gameplay/Abilities/ScWGameplayAbility_CommonSwing.h"

#include "AI/ScWTypes_AI.h"

#include "Animations/ScWAnimationsFunctionLibrary.h"

#include "Gameplay/ScWDamageType.h"
#include "Gameplay/ScWGameplayTags.h"
#include "Gameplay/Tasks/ScWAT_WaitDelay.h"
#include "Gameplay/Characters/ScWCharacter.h"
#include "Gameplay/Handhelds/ScWHandheld_Melee.h"
#include "Gameplay/Handhelds/ScWHandheldData_Melee.h"

UScWGameplayAbility_CommonSwing::UScWGameplayAbility_CommonSwing()
{
	{
		auto Tags = GetAssetTags();
		Tags.AddTag(FScWGameplayTags::Ability_Handheld_Swing);
		SetAssetTags(Tags);
	}
	CancelAbilitiesWithTag.AddTag(FScWGameplayTags::Ability_CancelBy_SwingActivate);
	ActivationOwnedTags.AddTag(FScWGameplayTags::State_Swinging);

	PreSwingMontageSectionIndex = 0;
	SwingMontageSectionIndex = 1;
	PostSwingMontageSectionIndex = 2;

	bLoopIfInputIsPressed = true;
}

//~ Begin Ability
void UScWGameplayAbility_CommonSwing::NativeActivateAbility_Commited(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UGameplayAbility
{
	ensureCancelAbilityReturn(OwnerCharacter);

	OwnerMelee = OwnerCharacter->GetHandheld<AScWHandheld_Melee>();
	ensureCancelAbilityReturn(OwnerMelee);
	
	OwnerMeleeData = OwnerMelee->GetMeleeDataAsset();
	ensureCancelAbilityReturn(OwnerMeleeData);

	BeginSwingSequence();
}

void UScWGameplayAbility_CommonSwing::BeginSwingSequence()
{
	float PreSwingDelay = BP_HandlePreSwing();

	UScWAT_WaitDelay* PreSwingWaitDelayTask = UScWAT_WaitDelay::WaitDelayOrFinishImmediately(this, PreSwingDelay);
	PreSwingWaitDelayTask->OnFinish.AddDynamic(this, &ThisClass::OnPreSwingDelayFinished);
	PreSwingWaitDelayTask->ReadyForActivation();
}

void UScWGameplayAbility_CommonSwing::OnPreSwingDelayFinished()
{
	float SwingDuration = BP_HandleBeginSwing();

	UScWAT_WaitDelay* PrePatternsWaitDelayTask = UScWAT_WaitDelay::WaitDelayOrFinishImmediately(this, SwingDuration);
	PrePatternsWaitDelayTask->OnFinish.AddDynamic(this, &ThisClass::OnSwingFinished);
	PrePatternsWaitDelayTask->ReadyForActivation();
}

void UScWGameplayAbility_CommonSwing::OnSwingFinished()
{
	float PostSwingDelay = BP_HandleEndSwing();

	if (bLoopIfInputIsPressed && IsAbilityInputPressed())
	{
		BeginSwingSequence();
	}
	else
	{
		UScWAT_WaitDelay* PrePatternsWaitDelayTask = UScWAT_WaitDelay::WaitDelayOrFinishImmediately(this, PostSwingDelay);
		PrePatternsWaitDelayTask->OnFinish.AddDynamic(this, &ThisClass::OnPostSwingDelayFinished);
		PrePatternsWaitDelayTask->ReadyForActivation();
	}
}

void UScWGameplayAbility_CommonSwing::OnPostSwingDelayFinished()
{
	BP_HandlePostSwing();
}

void UScWGameplayAbility_CommonSwing::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);

	TrySendAIMessageToOwner(FScWAIMessage::SwingEnded, !bInWasCancelled);
}
//~ End Ability

//~ Begin Melee
float UScWGameplayAbility_CommonSwing::BP_HandlePreSwing_Implementation()
{
	ensureCancelAbilityReturn(OwnerMelee, 0.0f);
	OwnerMelee->BP_PreSwing();

	const auto& CurrentSwingVariantData = OwnerMelee->GetCurrentSwingVariantData();
	const auto& CurrentSwingMontageData = CurrentSwingVariantData.MontageData;

	ensureCancelAbilityReturn(CurrentSwingMontageData.GetRelevantTimingMontage(), 0.0f);
	ensureCancelAbilityReturn(CurrentSwingMontageData.GetRelevantTimingMontage()->GetNumSections() >= 3, 0.0f);

	ensureCancelAbilityReturn(OwnerCharacter, 0.0f);
	UScWAnimationsFunctionLibrary::PlayCharacterMontagesFromData(OwnerCharacter, CurrentSwingMontageData);
	return UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingMontageData, PreSwingMontageSectionIndex);
}

float UScWGameplayAbility_CommonSwing::BP_HandleBeginSwing_Implementation()
{
	ensureCancelAbilityReturn(OwnerMelee, 0.0f);
	OwnerMelee->BP_BeginSwing(BP_GetSwingDamage(), BP_GetSwingDamageTypeClass());

	const auto& CurrentSwingVariantData = OwnerMelee->GetCurrentSwingVariantData();
	const auto& CurrentSwingMontageData = CurrentSwingVariantData.MontageData;
	return UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingMontageData, SwingMontageSectionIndex);
}

float UScWGameplayAbility_CommonSwing::BP_HandleEndSwing_Implementation()
{
	ensureCancelAbilityReturn(OwnerMelee, 0.0f);
	OwnerMelee->BP_EndSwing();

	const auto& CurrentSwingVariantData = OwnerMelee->GetCurrentSwingVariantData();
	const auto& CurrentSwingMontageData = CurrentSwingVariantData.MontageData;
	return UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingMontageData, PostSwingMontageSectionIndex);
}

void UScWGameplayAbility_CommonSwing::BP_HandlePostSwing_Implementation()
{
	K2_EndAbility();
}

float UScWGameplayAbility_CommonSwing::BP_GetSwingDamage_Implementation() const
{
	ensureReturn(OwnerMeleeData, 0.0f);
	return OwnerMeleeData->SwingBaseDamage;
}

TSubclassOf<UScWDamageType> UScWGameplayAbility_CommonSwing::BP_GetSwingDamageTypeClass_Implementation() const
{
	ensureReturn(OwnerMeleeData, UScWDamageType::StaticClass());
	return OwnerMeleeData->SwingBaseDamageTypeClass;
}
//~ End Melee
