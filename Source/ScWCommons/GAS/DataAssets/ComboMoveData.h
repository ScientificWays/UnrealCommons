// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ComboMoveData.generated.h"

/**
 *
 */
UCLASS(const, Blueprintable, BlueprintType)
class ATA_DREAMCOMETRUE_API UComboMoveData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	

	UComboMoveData();

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Game"))
	TSoftObjectPtr<UTexture2D> Image;
//~ End UI
};
