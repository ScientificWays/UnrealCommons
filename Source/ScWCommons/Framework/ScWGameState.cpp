// G.o.o.P Studios

#include "World/ATAGameState_Base.h"

#include "Characters/ATACharacter.h"

const FName AATAGameState_Base::InvalidTeam = TEXT("InvalidTeam");

AATAGameState_Base::AATAGameState_Base()
{
	bForceDefaultPlayerCharacterDataAsset = true;

	TeamMap.Add(TEXT("NoTeam"), FGenericTeamId::NoTeam);
	TeamMap.Add(TEXT("Player"), FGenericTeamId(1u));
	TeamMap.Add(TEXT("Enemy"), FGenericTeamId(2u));
}

//~ Begin Statics
AATAGameState_Base* AATAGameState_Base::TryGetAtaGameState(const UObject* InWCO)
{
	if (!InWCO)
	{
		UE_LOG(LogAtaGameplay, Error, TEXT("AATAGameState_Base::TryGetAtaGameState() World context is not valid!"));
		return nullptr;
	}

	UWorld* World = InWCO->GetWorld();
	if (!World)
	{
		UE_LOG(LogAtaGameplay, Error, TEXT("AATAGameState_Base::TryGetAtaGameState() World from context %s is not valid!"), *InWCO->GetName());
		return nullptr;
	}
	if (AATAGameState_Base* BaseGameState = World->GetGameState<AATAGameState_Base>())
	{
		return BaseGameState;
	}
	UE_LOG(LogAtaGameplay, Error, TEXT("AATAGameState_Base::TryGetAtaGameState() GameState from World %s is not of class AATAGameState_Base!"), *World->GetName());
	return nullptr;
}
//~ End Statics

//~ Begin DataAssets
const UCharacterData* AATAGameState_Base::K2_GetDataAssetForNewCharacter_Implementation(const AATACharacter* InCharacter) const
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
FGenericTeamId AATAGameState_Base::GetTeamId(const FName& InTeamName) const
{
	if (const FGenericTeamId* OutId = TeamMap.Find(InTeamName))
	{
		return *OutId;
	}
	return FGenericTeamId::NoTeam;
}

const FName& AATAGameState_Base::GetTeamName(FGenericTeamId InGenericTeamId) const
{
	if (const FName* OutName = TeamMap.FindKey(InGenericTeamId.GetId()))
	{
		return *OutName;
	}
	return InvalidTeam;
}
//~ End Teams
