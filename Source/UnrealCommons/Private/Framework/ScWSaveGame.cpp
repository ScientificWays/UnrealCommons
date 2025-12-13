// Scientific Ways

#include "Framework/ScWSaveGame.h"

#include "Framework/ScWGameInstance.h"

//~ Begin ScW SaveGame System
const FString& UScWSaveGame::GetCurrentSaveGameDataSlot(const UObject* InWCO)
{
	static const FString FallbackSlot = TEXT("DefaultSaveSlot");
	UScWGameInstance* GameInstance = Cast<UScWGameInstance>(UGameplayStatics::GetGameInstance(InWCO));
	ensureReturn(GameInstance, FallbackSlot);
	return GameInstance->CurrentSaveDataSlot;
}

UScWSaveGame* UScWSaveGame::GetCurrentSaveGameDataObject(const UObject* InWCO)
{
	UScWGameInstance* GameInstance = Cast<UScWGameInstance>(UGameplayStatics::GetGameInstance(InWCO));
	ensureReturn(GameInstance, nullptr);
	return GameInstance->CurrentSaveDataObject;
}

void UScWSaveGame::SaveCurrentSaveGameDataToCurrentSlot(const UObject* InWCO)
{
	ensureReturn(InWCO);
	UScWGameInstance* GameInstance = Cast<UScWGameInstance>(UGameplayStatics::GetGameInstance(InWCO));
	ensureReturn(GameInstance);

	ensureReturn(GameInstance->CurrentSaveDataObject);
	GameInstance->CurrentSaveDataObject->BP_PreSaveGameSave(InWCO);

	UGameplayStatics::SaveGameToSlot(GameInstance->CurrentSaveDataObject, GameInstance->CurrentSaveDataSlot, GameInstance->CurrentSaveDataUserIndex);
}

void UScWSaveGame::LoadCurrentSaveGameDataFromSlot(const UObject* InWCO, TSubclassOf<UScWSaveGame> InSaveGameClass, const FString& InSlot, int32 InUserIndex)
{
	ThisClass* LoadedData = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(InSlot, InUserIndex))
	{
		LoadedData = Cast<ThisClass>(UGameplayStatics::LoadGameFromSlot(InSlot, InUserIndex));
	}
	else
	{
		LoadedData = NewObject<ThisClass>(GetTransientPackage(), InSaveGameClass);
	}
	ensureReturn(LoadedData);
	ensureReturn(LoadedData->GetClass() == InSaveGameClass);

	ensureReturn(InWCO);
	UScWGameInstance* GameInstance = Cast<UScWGameInstance>(UGameplayStatics::GetGameInstance(InWCO));
	ensureReturn(GameInstance);
	GameInstance->CurrentSaveDataSlot = InSlot;
	GameInstance->CurrentSaveDataUserIndex = InUserIndex;
	GameInstance->CurrentSaveDataObject = LoadedData;

	GameInstance->CurrentSaveDataObject->BP_PostSaveGameLoad(InWCO);
}

void UScWSaveGame::ResetCurrentSaveGameData(const UObject* InWCO)
{
	ensureReturn(InWCO);
	UScWGameInstance* GameInstance = Cast<UScWGameInstance>(UGameplayStatics::GetGameInstance(InWCO));

	ensureReturn(GameInstance->CurrentSaveDataObject);
	GameInstance->CurrentSaveDataObject->BoolKeys.Empty();
	GameInstance->CurrentSaveDataObject->IntegerKeys.Empty();
	GameInstance->CurrentSaveDataObject->FloatKeys.Empty();
	GameInstance->CurrentSaveDataObject->VectorKeys.Empty();
	GameInstance->CurrentSaveDataObject->StringKeys.Empty();

	GameInstance->CurrentSaveDataObject->BP_ResetSaveGameData(InWCO);
}

#define DECLARE_GET_SET_CURRENT_SAVE_GAME_DATA(InType, InName) \
	InType UScWSaveGame::GetCurrentSaveGameData##InName(const UObject* InWCO, const FString& InKey, const InType& InDefaultValue) \
	{ \
		UScWSaveGame* CurrentSaveDataObject = GetCurrentSaveGameDataObject(InWCO); \
		ensureReturn(CurrentSaveDataObject, InDefaultValue); \
		return CurrentSaveDataObject->InName##Keys.Contains(InKey) ? CurrentSaveDataObject->InName##Keys[InKey] : InDefaultValue; \
	} \
	void UScWSaveGame::SetCurrentSaveGameData##InName(const UObject* InWCO, const FString& InKey, const InType& InValue) \
	{ \
		UScWSaveGame* CurrentSaveDataObject = GetCurrentSaveGameDataObject(InWCO); \
		ensureReturn(CurrentSaveDataObject); \
		CurrentSaveDataObject->InName##Keys.Add(InKey, InValue); \
	} \

DECLARE_GET_SET_CURRENT_SAVE_GAME_DATA(bool, Bool)
DECLARE_GET_SET_CURRENT_SAVE_GAME_DATA(int, Integer)
DECLARE_GET_SET_CURRENT_SAVE_GAME_DATA(float, Float)
DECLARE_GET_SET_CURRENT_SAVE_GAME_DATA(FVector, Vector)
DECLARE_GET_SET_CURRENT_SAVE_GAME_DATA(FString, String)
//~ End ScW SaveGame System
