// Scientific Ways

#include "Framework/ScWLevelScriptActor.h"

#include "Framework/ScWSaveGame.h"
#include "Framework/ScWGameInstance.h"

#include "Gameplay/Characters/ScWCharacter.h"

AScWLevelScriptActor::AScWLevelScriptActor()
{
	bTrySaveGameOnEndPlay = true;

	bSendBeginPlayDesignEvent = true;
	bSendEndPlayDesignEvent = true;
}

//~ Begin Statics
AScWLevelScriptActor* AScWLevelScriptActor::TryGetScWLevelScriptOfActor(const AActor* InActor)
{
	ensureReturn(InActor, nullptr);
	ensureReturn(InActor->GetLevel(), nullptr);
	return Cast<AScWLevelScriptActor>(InActor->GetLevel()->GetLevelScriptActor());
}
//~ End Statics

//~ Begin Initialize
void AScWLevelScriptActor::PostInitializeComponents() // AActor
{
	Super::PostInitializeComponents();

	APawn::OnPawnBeginPlay.AddUObject(this, &AScWLevelScriptActor::BP_OnPawnBeginPlay);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		GameInstance->GetOnPawnControllerChanged().AddDynamic(this, &AScWLevelScriptActor::BP_OnPawnControllerChanged);
	}
}

void AScWLevelScriptActor::BeginPlay() // AActor
{
	Super::BeginPlay();

	if (bSendBeginPlayDesignEvent)
	{
		BP_SendDefaultDesignAnalyticsEvent(TEXT("BeginPlay"));
	}
}

void AScWLevelScriptActor::EndPlay(const EEndPlayReason::Type InEndPlayReason) // AActor
{
	if (bSendEndPlayDesignEvent)
	{
		BP_SendDefaultDesignAnalyticsEvent(TEXT("EndPlay"));
	}
	if (bTrySaveGameOnEndPlay && !UScWSaveGame::GetCurrentSaveGameDataSlot(this).IsEmpty())
	{
		UScWSaveGame::SaveCurrentSaveGameDataToCurrentSlot(this);
	}
	Super::EndPlay(InEndPlayReason);
}
//~ End Initialize

//~ Begin Data Assets
const UScWCharacterData* AScWLevelScriptActor::BP_GetDataAssetForNewCharacter_Implementation(const AScWCharacter* InCharacter) const
{
	if (InCharacter)
	{
		if (InCharacter->IsPlayerCharacter())
		{
			if (!InCharacter->GetDataAsset() || (bForceDefaultPlayerCharacterDataAsset && DefaultPlayerCharacterDataAsset))
			{
				return DefaultPlayerCharacterDataAsset;
			}
		}
		return InCharacter->GetDataAsset();
	}
	return nullptr;
}
//~ End Data Assets

//~ Begin Pawns
//~ End Pawns

//~ Begin Levels
void AScWLevelScriptActor::LoadStreamLevelArraySync(const UObject* InWCO, const TArray<TSoftObjectPtr<UWorld>>& InLevelArray, bool bInMakeVisibleAfterLoad)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(InWCO, EGetWorldErrorMode::LogAndReturnNull))
	{
		int32 UUID = 0;
		for (const TSoftObjectPtr<UWorld> SampleLevel : InLevelArray)
		{
			FLatentActionInfo LatentActionInfo;
			LatentActionInfo.CallbackTarget = this;
			LatentActionInfo.UUID = UUID;
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(World, SampleLevel, bInMakeVisibleAfterLoad, false, LatentActionInfo);
			UUID++;
		}
		World->FlushLevelStreaming();
	}
}

void AScWLevelScriptActor::UnLoadStreamLevelArraySync(const UObject* InWCO, const TArray<TSoftObjectPtr<UWorld>>& InLevelArray)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(InWCO, EGetWorldErrorMode::LogAndReturnNull))
	{
		int32 UUID = 0;
		for (const TSoftObjectPtr<UWorld> SampleLevel : InLevelArray)
		{
			FLatentActionInfo LatentActionInfo;
			LatentActionInfo.CallbackTarget = this;
			LatentActionInfo.UUID = UUID;
			UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(World, SampleLevel, LatentActionInfo, false);
			UUID++;
		}
		World->FlushLevelStreaming();
	}
}
//~ End Levels

//~ Begin Analytics
FString AScWLevelScriptActor::BP_GetDefaultAnalyticsChapterName_Implementation() const
{
	return TEXT("Chapter1");
}

FString AScWLevelScriptActor::BP_GetDefaultAnalyticsMapName_Implementation() const
{
	return GetWorld() ? GetWorld()->GetMapName() : TEXT("UnknownMap");
}

FString AScWLevelScriptActor::BP_GetDefaultAnalyticsLocationName_Implementation(const FVector& InLocation) const
{
	return FString();
}

void AScWLevelScriptActor::BP_SendLevelProgressionAnalyticsEvent_Implementation(EGAProgressionStatus InType, const FVector& InEventLocation, const bool bInSendEventLocation) const
{
	UGameAnalytics* GameAnalytics = FGameAnalyticsModule::Get().GetInstance();
	ensureReturn(GameAnalytics);

	GameAnalytics->AddProgressionEvent(
		InType,
		BP_GetDefaultAnalyticsChapterName(),
		BP_GetDefaultAnalyticsMapName(),
		bInSendEventLocation ? BP_GetDefaultAnalyticsLocationName(InEventLocation) : FString()
	);
}

void AScWLevelScriptActor::BP_SendDefaultDesignAnalyticsEvent_Implementation(const FString& InEvent) const
{
	UGameAnalytics* GameAnalytics = FGameAnalyticsModule::Get().GetInstance();
	ensureReturn(GameAnalytics);

	auto CustomFields = FGACustomFields();
	CustomFields.Set(TEXT("ChapterName"), BP_GetDefaultAnalyticsChapterName());
	CustomFields.Set(TEXT("MapName"), BP_GetDefaultAnalyticsMapName());
	CustomFields.Set(TEXT("Event"), InEvent);
	GameAnalytics->AddDesignEvent(TEXT("DefaultLevelEvent"), CustomFields);
}
//~ End Analytics
