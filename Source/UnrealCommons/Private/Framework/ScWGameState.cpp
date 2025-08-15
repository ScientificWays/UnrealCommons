// Scientific Ways

#include "Framework/ScWGameState.h"

#include "Gameplay/Characters/ScWCharacter.h"

#include "Framework/ScWLevelScriptActor.h"

const FName AScWGameState::InvalidTeamName = TEXT("InvalidTeam");

AScWGameState::AScWGameState()
{
	bForceDefaultPlayerCharacterDataAsset = true;

	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, DefaultAddHealthGameplayEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonAddHealth");
	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, DefaultSetHealthGameplayEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonSetHealth");
	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, DefaultDeadStateGameplayEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonDeadState");
	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, DefaultApplyDamageGameplayEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonApplyDamage");
	
	TeamMap.Add(TEXT("NoTeam"), FGenericTeamId::NoTeam);
	TeamMap.Add(TEXT("Player"), FGenericTeamId(1u));
	TeamMap.Add(TEXT("Enemies"), FGenericTeamId(2u));
}

//~ Begin Statics
AScWGameState* AScWGameState::TryGetScWGameState(const UObject* InWCO)
{
	ensureReturn(InWCO, nullptr);
	UWorld* World = InWCO->GetWorld();

	ensureReturn(World, nullptr);
	AScWGameState* OutGameState = World->GetGameState<AScWGameState>();

	ensureReturn(OutGameState, nullptr);
	return OutGameState;
}
//~ End Statics

//~ Begin DataAssets
const UScWCharacterData* AScWGameState::BP_GetDataAssetForNewCharacter_Implementation(const AScWCharacter* InCharacter) const
{
	if (UWorld* World = GetWorld())
	{
		if (AScWLevelScriptActor* LevelScriptActor = Cast<AScWLevelScriptActor>(World->GetLevelScriptActor()))
		{
			if (const UScWCharacterData* OutCharacterData = LevelScriptActor->BP_GetDataAssetForNewCharacter(InCharacter))
			{
				return OutCharacterData;
			}
		}
	}
	if (InCharacter)
	{
		if (InCharacter->IsPlayerCharacter())
		{
			if (!InCharacter->GetDataAsset() || (bForceDefaultPlayerCharacterDataAsset && DefaultPlayerCharacterDataAsset))
			{
				return DefaultPlayerCharacterDataAsset;
			}
		}
		return InCharacter->GetDataAsset();
	}
	return nullptr;
}
//~ End DataAssets

//~ Begin Teams
bool AScWGameState::HasTeamName(const FName& InTeamName) const
{
	return TeamMap.Contains(InTeamName);
}

FGenericTeamId AScWGameState::GetTeamId(const FName& InTeamName) const
{
	const FGenericTeamId* OutTeamIdPtr = TeamMap.Find(InTeamName);
	ensureReturn(OutTeamIdPtr, FGenericTeamId::NoTeam);
	return *OutTeamIdPtr;
}

const FName& AScWGameState::GetTeamName(FGenericTeamId InGenericTeamId) const
{
	const FName* OutName = TeamMap.FindKey(InGenericTeamId.GetId());
	ensureReturn(OutName, InvalidTeamName);
	return *OutName;
}
//~ End Teams
