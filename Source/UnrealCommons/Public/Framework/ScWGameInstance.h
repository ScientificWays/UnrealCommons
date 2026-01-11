// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Engine/GameInstance.h"

#include "ScWGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerIsUsingGamepadSignature, AScWPlayerController*, InPlayerController, bool, bInIsUsingGamepad);

USTRUCT(BlueprintType)
struct FGameAnalyticsSettings
{
	GENERATED_BODY()

	UPROPERTY(Category = "Setup", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Your GameAnalytics Game Key - copy/paste from the GA website."))
	FString GameKey;

	UPROPERTY(Category = "Setup", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Your GameAnalytics Secret Key - copy/paste from the GA website."))
	FString SecretKey;

	UPROPERTY(Category = "Setup", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "The current version of the game. Updating the build name for each test version of the game will allow you to filter by build when viewing your data on the GA website."))
	FString Build = TEXT("0.1");

	UPROPERTY(Category = "Events", EditDefaultsOnly, BlueprintReadOnly)
	bool bEnableEventSubmission = true;

	UPROPERTY(Category = "CustomDimensions", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "List of custom dimensions 01."))
    TArray<FString> CustomDimensions01;

	UPROPERTY(Category = "CustomDimensions", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "List of custom dimensions 02."))
    TArray<FString> CustomDimensions02;

	UPROPERTY(Category = "CustomDimensions", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "List of custom dimensions 03."))
    TArray<FString> CustomDimensions03;

	UPROPERTY(Category = "ResourceTypes", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "List of Resource Currencies."))
    TArray<FString> ResourceCurrencies;

	UPROPERTY(Category = "ResourceTypes", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "List of Resource Item Types."))
    TArray<FString> ResourceItemTypes;

	UPROPERTY(Category = "Advanced", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Use manual session handling. Manually choose when to end and start a new session. Note initializing of the SDK will automatically start the first session."))
    bool bUseManualSessionHandling = false;

	UPROPERTY(Category = "Advanced", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Auto detect app version to use for build field (only for Android and iOS)."))
    bool bAutoDetectAppVersion = false;

	UPROPERTY(Category = "Advanced", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Disable device info (only Mac, Windows and Linux)."))
    bool bDisableDeviceInfo = false;

	UPROPERTY(Category = "Advanced", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Use automatic error reporting."))
    bool bUseErrorReporting = true;

	UPROPERTY(Category = "Advanced", EditDefaultsOnly, BlueprintReadOnly)
    bool bEnableFpsHistogram = false;

	UPROPERTY(Category = "Advanced", EditDefaultsOnly, BlueprintReadOnly)
	bool bEnableMemoryHistogram = false;

	UPROPERTY(Category = "Advanced", EditDefaultsOnly, BlueprintReadOnly)
	bool bEnableHealthHardwareInfo = false;

	UPROPERTY(Category = "Debug", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Show info messages from GA in builds (e.g. Xcode for iOS)."))
    bool bInfoLogBuild = true;

	UPROPERTY(Category = "Debug", EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Show full info messages from GA in builds (e.g. Xcode for iOS). Noet that this option includes long JSON messages sent to the server."))
    bool bVerboseLogBuild = false;
};

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Game Instance"))
class UNREALCOMMONS_API UScWGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UScWGameInstance();
	
//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO", DisplayName = "Try Get ScW Game Instance"))
	static UScWGameInstance* TryGetScWGameInstance(const UObject* InWCO);
//~ End Statics

//~ Begin Initialize
public:
	virtual void Init() override; // UGameInstance
	virtual void Shutdown() override; // UGameInstance
//~ End Initialize

//~ Begin Save
public:

	UPROPERTY(Category = "Save", BlueprintReadOnly)
	FString CurrentSaveDataSlot;

	UPROPERTY(Category = "Save", BlueprintReadOnly)
	int32 CurrentSaveDataUserIndex;

	UPROPERTY(Category = "Save", BlueprintReadOnly)
	TObjectPtr<class UScWSaveGame> CurrentSaveDataObject;
//~ End Save

//~ Begin Input
public:

	UPROPERTY(Category = "Input", BlueprintAssignable)
	FPlayerIsUsingGamepadSignature OnPlayerIsUsingGamepadChangedDelegate;
//~ End Input

//~ Begin Analytics
protected:

	UFUNCTION(Category = "Analytics", BlueprintNativeEvent)
	UGameAnalytics* BP_InitGameAnalytics();

	UPROPERTY(Category = "Analytics", EditDefaultsOnly, BlueprintReadOnly)
	bool bInitGameAnalytics;

	UPROPERTY(Category = "Analytics", EditDefaultsOnly, BlueprintReadOnly)
	FGameAnalyticsSettings GameAnalyticsSettings;
//~ End Analytics
};
