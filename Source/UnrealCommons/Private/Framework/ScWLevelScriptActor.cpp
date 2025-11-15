// Scientific Ways

#include "Framework/ScWLevelScriptActor.h"

#include "Gameplay/Characters/ScWCharacter.h"

AScWLevelScriptActor::AScWLevelScriptActor()
{

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


}
//~ End Initialize

//~ Begin DataAssets
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
//~ End DataAssets

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
