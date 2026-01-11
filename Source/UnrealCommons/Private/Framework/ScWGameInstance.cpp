// Scientific Ways

#include "Framework/ScWGameInstance.h"

#include "GameAnalytics.h"
#include "GameAnalyticsModule.h"

UScWGameInstance::UScWGameInstance()
{
	
}

//~ Begin Statics
UScWGameInstance* UScWGameInstance::TryGetScWGameInstance(const UObject* InWCO)
{
	ensureReturn(InWCO, nullptr);

	UWorld* World = InWCO->GetWorld();
	ensureReturn(World, nullptr);

	UScWGameInstance* OutGameInstance = World->GetGameInstance<UScWGameInstance>();
	ensureReturn(OutGameInstance, nullptr);
	return OutGameInstance;
}
//~ End Statics

//~ Begin Initialize
void UScWGameInstance::Init() // UGameInstance
{
	Super::Init();

	if (bInitGameAnalytics)
	{
		BP_InitGameAnalytics();
	}
}

void UScWGameInstance::Shutdown() // UGameInstance
{
	Super::Shutdown();


}
//~ End Initialize

//~ Begin Analytics
UGameAnalytics* UScWGameInstance::BP_InitGameAnalytics_Implementation()
{
	UGameAnalytics* GameAnalytics = FGameAnalyticsModule::Get().GetInstance();

	GameAnalytics->Initialize(GameAnalyticsSettings.GameKey, GameAnalyticsSettings.SecretKey);
	GameAnalytics->ConfigureBuild(GameAnalyticsSettings.Build);

	GameAnalytics->SetEnabledEventSubmission(GameAnalyticsSettings.bEnableEventSubmission);

	GameAnalytics->ConfigureAvailableCustomDimensions01(GameAnalyticsSettings.CustomDimensions01);
	GameAnalytics->ConfigureAvailableCustomDimensions02(GameAnalyticsSettings.CustomDimensions02);
	GameAnalytics->ConfigureAvailableCustomDimensions03(GameAnalyticsSettings.CustomDimensions03);

	GameAnalytics->ConfigureAvailableResourceCurrencies(GameAnalyticsSettings.ResourceCurrencies);
	GameAnalytics->ConfigureAvailableResourceItemTypes(GameAnalyticsSettings.ResourceItemTypes);

	GameAnalytics->SetEnabledManualSessionHandling(GameAnalyticsSettings.bUseManualSessionHandling);
	GameAnalytics->ConfigureAutoDetectAppVersion(GameAnalyticsSettings.bAutoDetectAppVersion);

	if (GameAnalyticsSettings.bDisableDeviceInfo)
	{
		GameAnalytics->DisableDeviceInfo();
	}
	GameAnalytics->SetEnabledErrorReporting(GameAnalyticsSettings.bUseErrorReporting);

	GameAnalytics->EnableFpsHistogram(GameAnalyticsSettings.bEnableFpsHistogram);
	GameAnalytics->EnableMemoryHistogram(GameAnalyticsSettings.bEnableMemoryHistogram);
	GameAnalytics->EnableHealthHardwareInfo(GameAnalyticsSettings.bEnableHealthHardwareInfo);

	GameAnalytics->SetEnabledInfoLog(GameAnalyticsSettings.bInfoLogBuild);
	GameAnalytics->SetEnabledVerboseLog(GameAnalyticsSettings.bVerboseLogBuild);
	return GameAnalytics;
}
//~ End Analytics
