// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_CommonDelegates.h"

#include "ScWGameState.generated.h"

/**
 * 
 */
UCLASS(Config = UnrealCommons, meta = (DisplayName = "[ScW] Game State"))
class UNREALCOMMONS_API AScWGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AScWGameState();
	
//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static AScWGameState* TryGetScWGameState(const UObject* InWCO);
//~ End Statics
	
//~ Begin Data Assets
public:

	// Default implementation tries to get data from ScW Level Script Actor first
	UFUNCTION(Category = "Data Assets", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetDataAssetForNewCharacter"))
	const class UScWCharacterData* BP_GetDataAssetForNewCharacter(const class AScWCharacter* InCharacter) const;

	UPROPERTY(Category = "Data Assets", EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const class UScWCharacterData> DefaultPlayerCharacterDataAsset;

	UPROPERTY(Category = "Data Assets", EditAnywhere, BlueprintReadWrite)
	bool bForceDefaultPlayerCharacterDataAsset;
//~ End Data Assets

//~ Begin Gameplay Ability System
public:

	UPROPERTY(Config, Category = "Gameplay Ability System", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DefaultSetHealthGameplayEffectClass;

	UPROPERTY(Config, Category = "Gameplay Ability System", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DefaultAddHealthGameplayEffectClass;

	UPROPERTY(Config, Category = "Gameplay Ability System", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DefaultDeadStateGameplayEffectClass;

	UPROPERTY(Config, Category = "Gameplay Ability System", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DefaultApplyDamageGameplayEffectClass;
//~ End Gameplay Ability System

//~ Begin Teams
public:

	UFUNCTION(Category = "Teams", BlueprintCallable)
	bool HasTeamName(const FName& InTeamName) const;

	UFUNCTION(Category = "Teams", BlueprintCallable)
	FGenericTeamId GetTeamId(const FName& InTeamName) const;

	UFUNCTION(Category = "Teams", BlueprintCallable)
	const FName& GetTeamName(FGenericTeamId InGenericTeamId) const;

	UPROPERTY(Category = "Teams", EditAnywhere, BlueprintReadWrite)
	TMap<FName, FGenericTeamId> TeamMap;

	static const FName InvalidTeamName;
//~ End Teams

//~ Begin Characters
public:

	UPROPERTY(Category = "Characters", BlueprintAssignable)
	FScWCharacterSignature OnCharacterDied;
//~ End Characters
};
