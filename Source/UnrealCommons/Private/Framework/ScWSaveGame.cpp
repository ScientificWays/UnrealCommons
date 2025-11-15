// Scientific Ways

#include "Framework/ScWSaveGame.h"

#include "Framework/ScWGameInstance.h"

//~ Begin Slot
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

void UScWSaveGame::SaveCurrentSaveGameDataToSlot(const UObject* InWCO, const FString& InSlot, int32 InUserIndex)
{
	ensureReturn(InWCO);
	UScWGameInstance* GameInstance = Cast<UScWGameInstance>(UGameplayStatics::GetGameInstance(InWCO));
	ensureReturn(GameInstance);

	ensureReturn(GameInstance->CurrentSaveDataObject);
	UGameplayStatics::SaveGameToSlot(GameInstance->CurrentSaveDataObject, InSlot, InUserIndex);
}

void UScWSaveGame::LoadCurrentSaveGameDataFromSlot(const UObject* InWCO, const FString& InSlot, int32 InUserIndex)
{
	ThisClass* LoadedData = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(InSlot, InUserIndex))
	{
		LoadedData = Cast<ThisClass>(UGameplayStatics::LoadGameFromSlot(InSlot, InUserIndex));
	}
	else
	{
		LoadedData = NewObject<ThisClass>(GetTransientPackage(), ThisClass::StaticClass());
	}
	ensureReturn(LoadedData);

	ensureReturn(InWCO);
	UScWGameInstance* GameInstance = Cast<UScWGameInstance>(UGameplayStatics::GetGameInstance(InWCO));
	ensureReturn(GameInstance);
	GameInstance->CurrentSaveDataObject = LoadedData;
}
//~ End Slot

//~ Begin Data
bool UScWSaveGame::GetCurrentSaveGameDataBool(const UObject* InWCO, const FString& InKey, const bool bInDefaultValue)
{
	UScWSaveGame* CurrentSaveDataObject = GetCurrentSaveGameDataObject(InWCO);
	ensureReturn(CurrentSaveDataObject, bInDefaultValue);
	return CurrentSaveDataObject->BoolKeys.Contains(InKey) ? CurrentSaveDataObject->BoolKeys[InKey] : bInDefaultValue;
}

int32 UScWSaveGame::GetCurrentSaveGameDataInteger(const UObject* InWCO, const FString& InKey, const int32 InDefaultValue)
{
	UScWSaveGame* CurrentSaveDataObject = GetCurrentSaveGameDataObject(InWCO);
	ensureReturn(CurrentSaveDataObject, InDefaultValue);
	return CurrentSaveDataObject->IntegerKeys.Contains(InKey) ? CurrentSaveDataObject->IntegerKeys[InKey] : InDefaultValue;
}

float UScWSaveGame::GetCurrentSaveGameDataFloat(const UObject* InWCO, const FString& InKey, const float InDefaultValue)
{
	UScWSaveGame* CurrentSaveDataObject = GetCurrentSaveGameDataObject(InWCO);
	ensureReturn(CurrentSaveDataObject, InDefaultValue);
	return CurrentSaveDataObject->FloatKeys.Contains(InKey) ? CurrentSaveDataObject->FloatKeys[InKey] : InDefaultValue;
}

FVector UScWSaveGame::GetCurrentSaveGameDataVector(const UObject* InWCO, const FString& InKey, const FVector& InDefaultValue)
{
	UScWSaveGame* CurrentSaveDataObject = GetCurrentSaveGameDataObject(InWCO);
	ensureReturn(CurrentSaveDataObject, InDefaultValue);
	return CurrentSaveDataObject->VectorKeys.Contains(InKey) ? CurrentSaveDataObject->VectorKeys[InKey] : InDefaultValue;
}

void UScWSaveGame::SetCurrentSaveGameDataBool(const UObject* InWCO, const FString& InKey, const bool bInValue)
{
	UScWSaveGame* CurrentSaveDataObject = GetCurrentSaveGameDataObject(InWCO);
	ensureReturn(CurrentSaveDataObject);
	CurrentSaveDataObject->BoolKeys.Add(InKey, bInValue);
}

void UScWSaveGame::SetCurrentSaveGameDataInteger(const UObject* InWCO, const FString& InKey, const int32 InValue)
{
	UScWSaveGame* CurrentSaveDataObject = GetCurrentSaveGameDataObject(InWCO);
	ensureReturn(CurrentSaveDataObject);
	CurrentSaveDataObject->IntegerKeys.Add(InKey, InValue);
}

void UScWSaveGame::SetCurrentSaveGameDataFloat(const UObject* InWCO, const FString& InKey, const float InValue)
{
	UScWSaveGame* CurrentSaveDataObject = GetCurrentSaveGameDataObject(InWCO);
	ensureReturn(CurrentSaveDataObject);
	CurrentSaveDataObject->FloatKeys.Add(InKey, InValue);
}

void UScWSaveGame::SetCurrentSaveGameDataVector(const UObject* InWCO, const FString& InKey, const FVector& InValue)
{
	UScWSaveGame* CurrentSaveDataObject = GetCurrentSaveGameDataObject(InWCO);
	ensureReturn(CurrentSaveDataObject);
	CurrentSaveDataObject->VectorKeys.Add(InKey, InValue);
}
//~ End Data
