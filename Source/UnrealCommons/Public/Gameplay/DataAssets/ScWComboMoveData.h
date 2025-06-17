// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWComboMoveData.generated.h"

/**
 *
 */
UCLASS(const, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Combo Move Data"))
class UNREALCOMMONS_API UScWComboMoveData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	

	UScWComboMoveData();

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Game"))
	TSoftObjectPtr<UTexture2D> Image;
//~ End UI
};
