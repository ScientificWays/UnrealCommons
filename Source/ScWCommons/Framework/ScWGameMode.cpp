// G.o.o.P Studios

#include "World/ATAGameMode_Base.h"

AATAGameMode_Base::AATAGameMode_Base()
{
	
}

AATAGameMode_Base* AATAGameMode_Base::TryGetAtaGameMode(const UObject* InWCO)
{
	if (!InWCO)
	{
		UE_LOG(LogAtaGameplay, Error, TEXT("AATAGameMode_Base::TryGetAtaGameMode() World context is not valid!"));
	}

	UWorld* World = InWCO->GetWorld();
	if (!World)
	{
		UE_LOG(LogAtaGameplay, Error, TEXT("AATAGameMode_Base::TryGetAtaGameMode() World from context %s is not valid!"), *InWCO->GetName());
	}
	if (AATAGameMode_Base* BaseGameMode = World->GetAuthGameMode<AATAGameMode_Base>())
	{
		return BaseGameMode;
	}
	UE_LOG(LogAtaGameplay, Error, TEXT("AATAGameMode_Base::TryGetAtaGameMode() GameMode from World %s is not of class AATAGameMode_Base!"), *World->GetName());
	return nullptr;
}
