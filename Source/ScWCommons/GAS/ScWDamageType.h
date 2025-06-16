// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ATADT_Base.generated.h"

/**
 *
 */
UCLASS(const, Blueprintable, BlueprintType)
class ATA_DREAMCOMETRUE_API UATADT_Base : public UDamageType
{
	GENERATED_BODY()

public:	

	UATADT_Base();

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName = FText();

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Game"))
	TSoftObjectPtr<UTexture2D> Image = nullptr;
//~ End UI

//~ Begin GAS
public:

	UPROPERTY(Category = "GAS", EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Game"))
	TSubclassOf<UGameplayEffect> ApplyDamageGameplayEffectClass = nullptr;
//~ End GAS

};
