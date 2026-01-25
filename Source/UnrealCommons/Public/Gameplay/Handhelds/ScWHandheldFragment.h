// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWHandheldFragment.generated.h"

/**
 *
 */
UCLASS(Const, Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew, meta = (DisplayName = "[ScW] Handheld Fragment Base"))
class UNREALCOMMONS_API UScWHandheldFragment : public UObject
{
	GENERATED_BODY()

public:	

	UScWHandheldFragment();
	
//~ Begin Swing
public:

	UFUNCTION(Category = "Swing", BlueprintCallable, BlueprintNativeEvent)
	float BP_ModifySwingBaseDamage(const class AScWHandheld* InHandheld, float InDamage) const;

	UFUNCTION(Category = "Swing", BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<class UScWDamageType> BP_ModifySwingBaseDamageTypeClass(const class AScWHandheld* InHandheld, TSubclassOf<class UScWDamageType> InDamageTypeClass) const;
//~ End Swing

//~ Begin Swing Variants
public:

	UFUNCTION(Category = "Swing Variants", BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "Modify Swing Variants"))
	void BP_ModifySwingVariants(const class AScWHandheld* InHandheld, TArray<FScWMeleeSwingVariantData>& InOutVariants) const;
//~ End Swing Variants
};
