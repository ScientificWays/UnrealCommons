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

//~ Begin ScW SaveGame System
public:

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static const FString& GetCurrentSaveGameDataSlot(const UObject* InWCO);

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static UScWSaveGame* GetCurrentSaveGameDataObject(const UObject* InWCO);

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InSlot"))
	static void SaveCurrentSaveGameDataToCurrentSlot(const UObject* InWCO);

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InSlot"))
	static void LoadCurrentSaveGameDataFromSlot(const UObject* InWCO, const FString& InSlot, int32 InUserIndex);

public:

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey, bInDefaultValue", KeyWords = "GetBoolSave, GetSaveDataBool"))
	static bool GetCurrentSaveGameDataBool(const UObject* InWCO, const FString& InKey, const bool& bInDefaultValue);

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey, bInDefaultValue", KeyWords = "GetIntegerSave, GetSaveDataInteger"))
	static int32 GetCurrentSaveGameDataInteger(const UObject* InWCO, const FString& InKey, const int32& InDefaultValue);

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey, bInDefaultValue", KeyWords = "GetFloatSave, GetSaveDataFloat"))
	static float GetCurrentSaveGameDataFloat(const UObject* InWCO, const FString& InKey, const float& InDefaultValue);

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey, bInDefaultValue", KeyWords = "GetVectorSave, GetSaveDataVector"))
	static FVector GetCurrentSaveGameDataVector(const UObject* InWCO, const FString& InKey, const FVector& InDefaultValue);

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey, bInDefaultValue", KeyWords = "GetStringSave, GetSaveDataString"))
	static FString GetCurrentSaveGameDataString(const UObject* InWCO, const FString& InKey, const FString& InDefaultValue);

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey, bInValue", KeyWords = "SaveBoolKey, SetSaveDataBool"))
	static void SetCurrentSaveGameDataBool(const UObject* InWCO, const FString& InKey, const bool& bInValue);

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey, InValue", KeyWords = "SaveIntegerKey, SetSaveDataInteger"))
	static void SetCurrentSaveGameDataInteger(const UObject* InWCO, const FString& InKey, const int32& InValue);

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey, InValue", KeyWords = "SaveFloatKey, SetSaveDataFloat"))
	static void SetCurrentSaveGameDataFloat(const UObject* InWCO, const FString& InKey, const float& InValue);

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey, InValue", KeyWords = "SaveVectorKey, SetSaveDataVector"))
	static void SetCurrentSaveGameDataVector(const UObject* InWCO, const FString& InKey, const FVector& InValue);

	UFUNCTION(Category = "ScW SaveGame System", BlueprintCallable, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InKey, InValue", KeyWords = "SaveStringKey, SetSaveDataString"))
	static void SetCurrentSaveGameDataString(const UObject* InWCO, const FString& InKey, const FString& InValue);
//~ End ScW SaveGame System

//~ Begin Data
protected:

	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite)
	TMap<FString, bool> BoolKeys;

	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite)
	TMap<FString, int32> IntegerKeys;

	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite)
	TMap<FString, float> FloatKeys;

	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite)
	TMap<FString, FVector> VectorKeys;

	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString> StringKeys;
//~ End Data
};
