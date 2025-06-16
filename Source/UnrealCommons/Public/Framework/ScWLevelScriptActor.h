// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "ScW Level Script Actor"))
class UNREALCOMMONS_API AScWLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:

	AScWLevelScriptActor();
	
//~ Begin Initialize
public:

	//UFUNCTION(Category = "Initialize", BlueprintImplementableEvent, BlueprintAuthorityOnly, meta = (DisplayName = "HandlePreGameSetup (Persistent Level)"))
	//void BP_HandlePreGameSetup_PersistentLevel(class AIDGameMode* InIDGameMode, class AIDGameState* InIDGameState);
	
protected:
	virtual void PreInitializeComponents() override; // AActor
	virtual void BeginPlay() override; // AActor
//~ End Initialize
	
//~ Begin DataAssets
public:

	// Called from ScW Game State default implementation of K2_GetDataAssetForNewCharacter()
	UFUNCTION(Category = "DataAssets", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetDataAssetForNewCharacter"))
	const class UScWCharacterData* K2_GetDataAssetForNewCharacter(const class AScWCharacter* InCharacter) const;

	UPROPERTY(Category = "DataAssets", EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const class UScWCharacterData> DefaultPlayerCharacterDataAsset;

	UPROPERTY(Category = "DataAssets", EditAnywhere, BlueprintReadWrite)
	bool bForceDefaultPlayerCharacterDataAsset;
//~ End DataAssets

//~ Begin Requests
public:

	//UFUNCTION(Category = "Requests", BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, meta = (DisplayName = "HandleCustomRequest (ScWLevelScriptActor)", AutoCreateRefTerm = "InRequestType"))
	//int32 BP_HandleCustomRequest(const FName& InRequestType, UObject* InOptionalObject = nullptr);
//~ End Requests

//~ Begin Events
public:

	// Call order: Ended->Started, AIDGameMode->AScWLevelScriptActor->AIDGameState
	//UFUNCTION(Category = "Phases", BlueprintImplementableEvent, meta = (DisplayName = "OnPhaseStarted"))
	//void BP_OnPhaseStarted(const FName& InPhase);

	// Call order: Ended->Started, AIDGameMode->AScWLevelScriptActor->AIDGameState
	//UFUNCTION(Category = "Phases", BlueprintImplementableEvent, meta = (DisplayName = "OnPhaseEnded"))
	//void BP_OnPhaseEnded(const FName& InPhase);
	
	//UFUNCTION(Category = "Events", BlueprintImplementableEvent, meta = (DisplayName = "OnPlayerCharacterSpawn"))
	//void BP_OnPlayerCharacterSpawn(class APlayerCharacter* InPlayerCharacter);
//~ End Events

//~ Begin Levels
public:

	UFUNCTION(Category = "Levels", BlueprintCallable, meta = (WorldContext = "InWCO", DisplayName = "LoadStreamLevelArraySync (by Object Reference)"))
	void LoadStreamLevelArraySync(const UObject* InWCO, const TArray<TSoftObjectPtr<UWorld>>& InLevelArray, bool bInMakeVisibleAfterLoad = true);

	UFUNCTION(Category = "Levels", BlueprintCallable, meta = (WorldContext = "InWCO", DisplayName = "UnLoadStreamLevelArraySync (by Object Reference)"))
	void UnLoadStreamLevelArraySync(const UObject* InWCO, const TArray<TSoftObjectPtr<UWorld>>& InLevelArray);
//~ End Levels
};
