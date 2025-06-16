// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWDamageType.generated.h"

/**
 *
 */
UCLASS(const, Blueprintable, BlueprintType)
class UNREALCOMMONS_API UScWDamageType : public UDamageType
{
	GENERATED_BODY()

public:	

	UScWDamageType();

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
