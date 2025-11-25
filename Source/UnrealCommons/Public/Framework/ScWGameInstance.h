// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Engine/GameInstance.h"

#include "ScWGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerIsUsingGamepadSignature, AScWPlayerController*, InPlayerController, bool, bInIsUsingGamepad);

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
};
