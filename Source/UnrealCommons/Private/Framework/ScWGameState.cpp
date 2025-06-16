// Scientific Ways

#include "Framework/ScWGameState.h"

#include "Characters/ScWCharacter.h"

const FName AScWGameState::InvalidTeam = TEXT("InvalidTeam");

AScWGameState::AScWGameState()
{
	bForceDefaultPlayerCharacterDataAsset = true;

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
	UE_LOG(LogScWGameplay, Error, TEXT("AScWGameState::TryGetScWGameState() GameState from World %s is not of class AScWGameState!"), *World->GetName());
	return nullptr;
}
//~ End Statics

//~ Begin DataAssets
const UScWCharacterData* AScWGameState::K2_GetDataAssetForNewCharacter_Implementation(const AScWCharacter* InCharacter) const
{
	if (!InCharacter)
	{
		return nullptr;
	}
	if (InCharacter->IsPlayerControlled())
	{
		if (!InCharacter->GetDataAsset() || (bForceDefaultPlayerCharacterDataAsset && DefaultPlayerCharacterDataAsset))
		{
			return DefaultPlayerCharacterDataAsset;
		}
	}
	return InCharacter->GetDataAsset();
}
//~ End DataAssets

//~ Begin Teams
FGenericTeamId AScWGameState::GetTeamId(const FName& InTeamName) const
{
	if (const FGenericTeamId* OutId = TeamMap.Find(InTeamName))
	{
		return *OutId;
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
