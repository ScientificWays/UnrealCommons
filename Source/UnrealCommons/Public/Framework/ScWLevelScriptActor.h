// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "GameAnalytics.h"
#include "GameAnalyticsModule.h"

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
	virtual void EndPlay(const EEndPlayReason::Type InEndPlayReason) override; // AActor
//~ End Initialize

//~ Begin Save Game
public:

	UPROPERTY(Category = "Save Game", EditAnywhere, BlueprintReadWrite)
	bool bTrySaveGameOnEndPlay;
//~ End Save Game
	
//~ Begin Pawns
public:

	UFUNCTION(Category = "Pawns", BlueprintImplementableEvent, meta = (DisplayName = "OnPawnBeginPlay", KeyWords = "OnCharacterBeginPlay"))
	void BP_OnPawnBeginPlay(APawn* InPawn);

	UFUNCTION(Category = "Pawns", BlueprintImplementableEvent, meta = (DisplayName = "OnPawnControllerChanged", KeyWords = "OnCharacterControllerChanged"))
	void BP_OnPawnControllerChanged(APawn* InPawn, AController* InController);
//~ End Pawns

//~ Begin Players
public:

	UFUNCTION(Category = "Players", BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "Choose Player Start"))
	AActor* BP_ChoosePlayerStart(AController* InPlayer) const;
//~ End Players

//~ Begin Data Assets
public:

	// Called from ScW Game State default implementation of BP_GetDataAssetForNewCharacter()
	UFUNCTION(Category = "Data Assets", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetDataAssetForNewCharacter"))
	const class UScWCharacterData* BP_GetDataAssetForNewCharacter(const class AScWCharacter* InCharacter) const;

	UPROPERTY(Category = "Data Assets", EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const class UScWCharacterData> DefaultPlayerCharacterDataAsset;

	UPROPERTY(Category = "Data Assets", EditAnywhere, BlueprintReadWrite)
	bool bForceDefaultPlayerCharacterDataAsset;
//~ End Data Assets

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

//~ Begin Audio
public:

	UFUNCTION(Category = "Audio", BlueprintCallable, meta = (AutoCreateRefTerm = "InLayer", InLayer = "Default"))
	UAudioComponent* GetSound2D_AtLayer(const FName& InLayer) const { return AudioLayerMap.Contains(InLayer) ? AudioLayerMap[InLayer] : nullptr; }

	UFUNCTION(Category = "Audio", BlueprintCallable, meta = (AutoCreateRefTerm = "InLayer", InLayer = "Default", AdvancedDisplay = "InPitchMultiplier, InStartTime, InConcurrencySettings, bInAutoDestroy"))
	UAudioComponent* PlaySound2D_AtLayer(const FName& InLayer, USoundBase* InSoundAsset, float InFadeInDuration = 0.0f, float InVolumeMultiplier = 1.0f, float InPitchMultiplier = 1.0f, float InStartTime = 0.0f, USoundConcurrency* InConcurrencySettings = nullptr, bool bInAutoDestroy = true);

	UFUNCTION(Category = "Audio", BlueprintCallable, meta = (AutoCreateRefTerm = "InLayer", InLayer = "Default"))
	bool StopSound2D_AtLayer(const FName& InLayer, float InFadeOutDuration = 1.0);

protected:
	void OnAudioComponentFinished(UAudioComponent* InAudioComponent);

	UPROPERTY(Category = "Audio", VisibleInstanceOnly, BlueprintReadOnly)
	TMap<FName, TObjectPtr<UAudioComponent>> AudioLayerMap;
//~ End Audio
	
//~ Begin Progression
public:

	UFUNCTION(Category = "Progression", BlueprintImplementableEvent, meta = (DisplayName = "OnPlayerProgressionTaskMeterValueChanged"))
	void BP_OnPlayerProgressionTaskMeterValueChanged(class AScWPlayerState* InPlayerState, const class UScWProgressionTaskData* InTaskData, const FName& InMeterName, const int32 InValue);

	UFUNCTION(Category = "Progression", BlueprintImplementableEvent, meta = (DisplayName = "OnPlayerCompletedProgressionTaskMeter"))
	void BP_OnPlayerCompletedProgressionTaskMeter(class AScWPlayerState* InPlayerState, const class UScWProgressionTaskData* InTaskData, const FName& InMeterName);

	UFUNCTION(Category = "Progression", BlueprintImplementableEvent, meta = (DisplayName = "OnPlayerCompletedProgressionTask"))
	void BP_OnPlayerCompletedProgressionTask(class AScWPlayerState* InPlayerState, const class UScWProgressionTaskData* InTaskData);
//~ End Progression

//~ Begin Analytics
public:

	UFUNCTION(Category = "Analytics", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Default Analytics Chapter Name"))
	FString BP_GetDefaultAnalyticsChapterName() const;

	UFUNCTION(Category = "Analytics", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Default Analytics Map Name"))
	FString BP_GetDefaultAnalyticsMapName() const;

	UFUNCTION(Category = "Analytics", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Default Analytics Location Name"))
	FString BP_GetDefaultAnalyticsLocationName(const FVector& InLocation) const;

	UFUNCTION(Category = "Analytics", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Send Level Start Analytics Event", AutoCreateRefTerm = "InEventLocation"))
	void BP_SendLevelProgressionAnalyticsEvent(EGAProgressionStatus InType, const FVector& InEventLocation, const bool bInSendEventLocation = false) const;

	UFUNCTION(Category = "Analytics", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Send Default Design Analytics Event"))
	void BP_SendDefaultDesignAnalyticsEvent(const FString& InEvent) const;

	UPROPERTY(Category = "Analytics", EditAnywhere, BlueprintReadWrite)
	bool bSendBeginPlayDesignEvent;

	UPROPERTY(Category = "Analytics", EditAnywhere, BlueprintReadWrite)
	bool bSendEndPlayDesignEvent;
//~ End Analytics
};
