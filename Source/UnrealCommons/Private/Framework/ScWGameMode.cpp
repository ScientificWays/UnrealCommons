// Scientific Ways

#include "Framework/ScWGameMode.h"

#include "Framework/ScWSaveGame.h"
#include "Framework/ScWGameState.h"
#include "Framework/ScWGameSession.h"
#include "Framework/ScWPlayerState.h"
#include "Framework/ScwGameInstance.h"
#include "Framework/ScWPlayerController.h"
#include "Framework/ScWLevelScriptActor.h"

#include "Gameplay/Characters/ScWCharacter.h"

AScWGameMode::AScWGameMode()
{
	GameStateClass = AScWGameState::StaticClass();
	DefaultPawnClass = AScWCharacter::StaticClass();
	PlayerStateClass = AScWPlayerState::StaticClass();
	GameSessionClass = AScWGameSession::StaticClass();
	PlayerControllerClass = AScWPlayerController::StaticClass();

	DefaultPlayerName = FText::FromString(TEXT("zanayn"));

	DefaultSaveGameSlot = TEXT("DefaultSaveGame");
}

//~ Begin Statics
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
//~ End Statics

//~ Begin Initialize
void AScWGameMode::InitGame(const FString& InMapName, const FString& InOptions, FString& InErrorMessage) // AGameStateBase
{
	Super::InitGame(InMapName, InOptions, InErrorMessage);

	BP_LoadSaveGameData();
}
//~ End Initialize

//~ Begin Players
AActor* AScWGameMode::ChoosePlayerStart_Implementation(AController* InPlayer)
{
	if (UWorld* World = GetWorld())
	{
		if (AScWLevelScriptActor* LevelScriptActor = Cast<AScWLevelScriptActor>(World->GetLevelScriptActor()))
		{
			if (AActor* LevelPlayerStart = LevelScriptActor->BP_ChoosePlayerStart(InPlayer))
			{
				return LevelPlayerStart;
			}
		}
	}
	return Super::ChoosePlayerStart_Implementation(InPlayer);
}
//~ End Players

//~ Begin Save Game
void AScWGameMode::BP_LoadSaveGameData_Implementation()
{
	UScWGameInstance* GameInstance = Cast<UScWGameInstance>(GetGameInstance());
	ensureReturn(GameInstance);

	if (DefaultSaveGameSlot.IsEmpty())
	{
		// TODO: Unload save game
	}
	else if (GameInstance->CurrentSaveDataSlot != DefaultSaveGameSlot)
	{
		ensureReturn(SaveGameClass);
		return UScWSaveGame::LoadCurrentSaveGameDataFromSlot(this, SaveGameClass, DefaultSaveGameSlot, 0);
	}
}
//~ End Save Game
