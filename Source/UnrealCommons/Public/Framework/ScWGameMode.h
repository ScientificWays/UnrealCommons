// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWGameMode.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Game Mode"))
class UNREALCOMMONS_API AScWGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AScWGameMode();
	
//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static AScWGameMode* TryGetScWGameMode(const UObject* InWCO);
//~ End Statics
	
//~ Begin Initialize
protected:
	virtual void InitGame(const FString& InMapName, const FString& InOptions, FString& InErrorMessage); // AGameModeBase
//~ End Initialize

//~ Begin Players
protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* InPlayer) override; // AGameModeBase
//~ End Players

//~ Begin Save Game
public:

	UFUNCTION(Category = "Save Game", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Load Save Game Data"))
	void BP_LoadSaveGameData();

	UPROPERTY(Category = "Save Game", EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UScWSaveGame> SaveGameClass;

	UPROPERTY(Category = "Save Game", EditAnywhere, BlueprintReadOnly)
	FString DefaultSaveGameSlot;
//~ End Save Game
};
