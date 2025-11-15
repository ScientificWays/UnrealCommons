// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "GameFramework/SaveGame.h"

#include "ScWSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOMMONS_API UScWSaveGame : public USaveGame
{
	GENERATED_BODY()

//~ Begin Slot
public:

	UFUNCTION(Category = "Slot", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static const FString& GetCurrentSaveGameDataSlot(const UObject* InWCO);

	UFUNCTION(Category = "Slot", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static UScWSaveGame* GetCurrentSaveGameDataObject(const UObject* InWCO);

	UFUNCTION(Category = "Slot", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InSlot"))
	static void SaveCurrentSaveGameDataToSlot(const UObject* InWCO, const FString& InSlot, int32 InUserIndex);

	UFUNCTION(Category = "Slot", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InSlot"))
	static void LoadCurrentSaveGameDataFromSlot(const UObject* InWCO, const FString& InSlot, int32 InUserIndex);
//~ End Slot

//~ Begin Data
public:

	UFUNCTION(Category = "Data", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey", KeyWords = "GetBoolSave"))
	static bool GetCurrentSaveGameDataBool(const UObject* InWCO, const FString& InKey, const bool bInDefaultValue);

	UFUNCTION(Category = "Data", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey", KeyWords = "GetIntegerSave"))
	static int32 GetCurrentSaveGameDataInteger(const UObject* InWCO, const FString& InKey, const int32 InDefaultValue);

	UFUNCTION(Category = "Data", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey", KeyWords = "GetFloatSave"))
	static float GetCurrentSaveGameDataFloat(const UObject* InWCO, const FString& InKey, const float InDefaultValue);

	UFUNCTION(Category = "Data", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey", KeyWords = "GetVectorSave"))
	static FVector GetCurrentSaveGameDataVector(const UObject* InWCO, const FString& InKey, const FVector& InDefaultValue);

	UFUNCTION(Category = "Data", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey", KeyWords = "SaveBoolKey"))
	static void SetCurrentSaveGameDataBool(const UObject* InWCO, const FString& InKey, const bool bInValue);

	UFUNCTION(Category = "Data", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey", KeyWords = "SaveIntegerKey"))
	static void SetCurrentSaveGameDataInteger(const UObject* InWCO, const FString& InKey, const int32 InValue);

	UFUNCTION(Category = "Data", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey", KeyWords = "SaveFloatKey"))
	static void SetCurrentSaveGameDataFloat(const UObject* InWCO, const FString& InKey, const float InValue);

	UFUNCTION(Category = "Data", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey", KeyWords = "SaveVectorKey"))
	static void SetCurrentSaveGameDataVector(const UObject* InWCO, const FString& InKey, const FVector& InValue);

protected:

	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite)
	TMap<FString, bool> BoolKeys;

	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite)
	TMap<FString, int32> IntegerKeys;

	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite)
	TMap<FString, float> FloatKeys;

	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite)
	TMap<FString, FVector> VectorKeys;
//~ End Data
};
