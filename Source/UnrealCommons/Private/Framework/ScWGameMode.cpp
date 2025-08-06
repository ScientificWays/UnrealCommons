// Scientific Ways

#include "Framework/ScWGameMode.h"

#include "Framework/ScWGameState.h"
#include "Framework/ScWGameSession.h"
#include "Framework/ScWPlayerState.h"
#include "Framework/ScWPlayerController.h"

#include "Gameplay/Characters/ScWCharacter.h"

AScWGameMode::AScWGameMode()
{
	GameStateClass = AScWGameState::StaticClass();
	DefaultPawnClass = AScWCharacter::StaticClass();
	PlayerStateClass = AScWPlayerState::StaticClass();
	GameSessionClass = AScWGameSession::StaticClass();
	PlayerControllerClass = AScWPlayerController::StaticClass();

	DefaultPlayerName = FText::FromString(TEXT("zanayn"));
}

AScWGameMode* AScWGameMode::TryGetScWGameMode(const UObject* InWCO)
{
	if (!InWCO)
	{
		UE_LOG(LogScWGameplay, Error, TEXT("AScWGameMode::TryGetScWGameMode() World context is not valid!"));
	}

	UWorld* World = InWCO->GetWorld();
	if (!World)
	{
		UE_LOG(LogScWGameplay, Error, TEXT("AScWGameMode::TryGetScWGameMode() World from context %s is not valid!"), *InWCO->GetName());
	}
	if (AScWGameMode* OutGameMode = World->GetAuthGameMode<AScWGameMode>())
	{
		return OutGameMode;
	}
	UE_LOG(LogScWGameplay, Error, TEXT("AScWGameMode::TryGetScWGameMode() GameMode from World %s is not of class AScWGameMode!"), *World->GetName());
	return nullptr;
}
