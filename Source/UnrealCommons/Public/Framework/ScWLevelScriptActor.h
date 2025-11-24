// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Level Script Actor"))
class UNREALCOMMONS_API AScWLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:

	AScWLevelScriptActor();
	
//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable)
	static AScWLevelScriptActor* TryGetScWLevelScriptOfActor(const AActor* InActor);
//~ End Statics

//~ Begin Initialize
public:

	//UFUNCTION(Category = "Initialize", BlueprintImplementableEvent, BlueprintAuthorityOnly, meta = (DisplayName = "HandlePreGameSetup (Persistent Level)"))
	//void BP_HandlePreGameSetup_PersistentLevel(class AIDGameMode* InIDGameMode, class AIDGameState* InIDGameState);
	
protected:
	virtual void PostInitializeComponents() override; // AActor
	virtual void BeginPlay() override; // AActor
//~ End Initialize
	
//~ Begin Pawns
public:

	UFUNCTION(Category = "Pawns", BlueprintImplementableEvent, meta = (DisplayName = "OnPawnBeginPlay", KeyWords = "OnCharacterBeginPlay"))
	void BP_OnPawnBeginPlay(APawn* InPawn);

	UFUNCTION(Category = "Pawns", BlueprintImplementableEvent, meta = (DisplayName = "OnPawnControllerChanged", KeyWords = "OnCharacterControllerChanged"))
	void BP_OnPawnControllerChanged(APawn* InPawn, AController* InController);
//~ End Pawns

//~ Begin DataAssets
public:

	// Called from ScW Game State default implementation of BP_GetDataAssetForNewCharacter()
	UFUNCTION(Category = "DataAssets", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetDataAssetForNewCharacter"))
	const class UScWCharacterData* BP_GetDataAssetForNewCharacter(const class AScWCharacter* InCharacter) const;

	UPROPERTY(Category = "DataAssets", EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const class UScWCharacterData> DefaultPlayerCharacterDataAsset;

	UPROPERTY(Category = "DataAssets", EditAnywhere, BlueprintReadWrite)
	bool bForceDefaultPlayerCharacterDataAsset;
//~ End DataAssets

//~ Begin Events
public:

	UFUNCTION(Category = "Events", BlueprintCallable, BlueprintImplementableEvent, BlueprintAuthorityOnly, meta = (DisplayName = "HandleCustomEvent (ScWLevelScriptActor)", AutoCreateRefTerm = "InEventName", ReturnDisplayName = "OutReturnCode"))
	int32 BP_HandleCustomEvent(const FName& InEventName, UObject* InOptionalObject = nullptr);
//~ End Events

//~ Begin Levels
public:

	UFUNCTION(Category = "Levels", BlueprintCallable, meta = (WorldContext = "InWCO", DisplayName = "LoadStreamLevelArraySync (by Object Reference)"))
	void LoadStreamLevelArraySync(const UObject* InWCO, const TArray<TSoftObjectPtr<UWorld>>& InLevelArray, bool bInMakeVisibleAfterLoad = true);

	UFUNCTION(Category = "Levels", BlueprintCallable, meta = (WorldContext = "InWCO", DisplayName = "UnLoadStreamLevelArraySync (by Object Reference)"))
	void UnLoadStreamLevelArraySync(const UObject* InWCO, const TArray<TSoftObjectPtr<UWorld>>& InLevelArray);
//~ End Levels
	
//~ Begin Progression
public:

	UFUNCTION(Category = "Progression", BlueprintImplementableEvent, meta = (DisplayName = "OnPlayerProgressionTaskMeterValueChanged"))
	void BP_OnPlayerProgressionTaskMeterValueChanged(class AScWPlayerState* InPlayerState, const class UScWProgressionTaskData* InTaskData, const FName& InMeterName, const int32 InValue);

	UFUNCTION(Category = "Progression", BlueprintImplementableEvent, meta = (DisplayName = "OnPlayerCompletedProgressionTaskMeter"))
	void BP_OnPlayerCompletedProgressionTaskMeter(class AScWPlayerState* InPlayerState, const class UScWProgressionTaskData* InTaskData, const FName& InMeterName);

	UFUNCTION(Category = "Progression", BlueprintImplementableEvent, meta = (DisplayName = "OnPlayerCompletedProgressionTask"))
	void BP_OnPlayerCompletedProgressionTask(class AScWPlayerState* InPlayerState, const class UScWProgressionTaskData* InTaskData);
//~ End Progression
};
