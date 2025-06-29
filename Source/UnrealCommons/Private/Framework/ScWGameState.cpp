// Scientific Ways

#include "Framework/ScWGameState.h"

#include "Gameplay/Characters/ScWCharacter.h"

#include "Framework/ScWLevelScriptActor.h"

const FName AScWGameState::InvalidTeam = TEXT("InvalidTeam");

AScWGameState::AScWGameState()
{
	bForceDefaultPlayerCharacterDataAsset = true;

	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, DefaultAddHealthGameplayEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonAddHealth");
	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, DefaultSetHealthGameplayEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonSetHealth");
	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, DefaultDeadStateGameplayEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonDeadState");
	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, DefaultApplyDamageGameplayEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonApplyDamage");
	
	TeamMap.Add(TEXT("NoTeam"), FGenericTeamId::NoTeam);
	TeamMap.Add(TEXT("Player"), FGenericTeamId(1u));
	TeamMap.Add(TEXT("Enemy"), FGenericTeamId(2u));
}

//~ Begin Statics
AScWGameState* AScWGameState::TryGetScWGameState(const UObject* InWCO)
{
	if (!InWCO)
	{
		UE_LOG(LogScWGameplay, Error, TEXT("AScWGameState::TryGetScWGameState() World context is not valid!"));
		return nullptr;
	}

	UWorld* World = InWCO->GetWorld();
	if (!World)
	{
		UE_LOG(LogScWGameplay, Error, TEXT("AScWGameState::TryGetScWGameState() World from context %s is not valid!"), *InWCO->GetName());
		return nullptr;
	}
	if (AScWGameState* OutGameState = World->GetGameState<AScWGameState>())
	{
		return OutGameState;
	}
	UE_LOG(LogScWGameplay, Error, TEXT("AScWGameState::TryGetScWGameState() GameState from %s is not of class AScWGameState!"), *World->GetName());
	return nullptr;
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
FGenericTeamId AScWGameState::GetTeamId(const FName& InTeamName) const
{
	if (const FGenericTeamId* OutTeamIdPtr = TeamMap.Find(InTeamName))
	{
		return *OutTeamIdPtr;
	}
	return FGenericTeamId::NoTeam;
}

const FName& AScWGameState::GetTeamName(FGenericTeamId InGenericTeamId) const
{
	if (const FName* OutName = TeamMap.FindKey(InGenericTeamId.GetId()))
	{
		return *OutName;
	}
	return InvalidTeam;
}
//~ End Teams
