// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ATAGameState_Base.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ATA_DREAMCOMETRUE_API AATAGameState_Base : public AGameStateBase
{
	GENERATED_BODY()

public:

	AATAGameState_Base();
	
//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static AATAGameState_Base* TryGetAtaGameState(const UObject* InWCO);
//~ End Statics
	
//~ Begin DataAssets
public:

	UFUNCTION(Category = "DataAssets", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetDataAssetForNewCharacter"))
	const UCharacterData* K2_GetDataAssetForNewCharacter(const class AATACharacter* InCharacter) const;

	UPROPERTY(Category = "DataAssets", EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const class UCharacterData> DefaultPlayerCharacterDataAsset;

	UPROPERTY(Category = "DataAssets", EditAnywhere, BlueprintReadWrite)
	bool bForceDefaultPlayerCharacterDataAsset;
//~ End DataAssets

//~ Begin GAS
public:

	UPROPERTY(Category = "GAS", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DefaultSetHealthGameplayEffectClass;

	UPROPERTY(Category = "GAS", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DefaultAddHealthGameplayEffectClass;

	UPROPERTY(Category = "GAS", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DefaultApplyDamageGameplayEffectClass;
//~ End GAS

//~ Begin Teams
public:

	UFUNCTION(Category = "Teams", BlueprintCallable)
	FGenericTeamId GetTeamId(const FName& InTeamName) const;

	UFUNCTION(Category = "Teams", BlueprintCallable)
	const FName& GetTeamName(FGenericTeamId InGenericTeamId) const;

	UPROPERTY(Category = "Teams", EditAnywhere, BlueprintReadWrite)
	TMap<FName, FGenericTeamId> TeamMap;

	static const FName InvalidTeam;
//~ End Teams
};
