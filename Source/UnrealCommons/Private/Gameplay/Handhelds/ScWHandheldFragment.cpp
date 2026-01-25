// Scientific Ways

#include "Gameplay/Handhelds/ScWHandheldFragment.h"

UScWHandheldFragment::UScWHandheldFragment()
{
	
}

//~ Begin Swing
float UScWHandheldFragment::BP_ModifySwingBaseDamage_Implementation(const class AScWHandheld* InHandheld, float InDamage) const
{
	ensureReturn(InHandheld, InDamage);
	return InDamage;
}

TSubclassOf<class UScWDamageType> UScWHandheldFragment::BP_ModifySwingBaseDamageTypeClass_Implementation(const class AScWHandheld* InHandheld, TSubclassOf<class UScWDamageType> InDamageTypeClass) const
{
	ensureReturn(InHandheld, InDamageTypeClass);
	return InDamageTypeClass;
}
//~ End Swing

//~ Begin Swing Variants
void UScWHandheldFragment::BP_ModifySwingVariants_Implementation(const class AScWHandheld* InHandheld, TArray<FScWMeleeSwingVariantData>& InOutVariants) const
{
	ensureReturn(InHandheld);
}
//~ End Swing Variants
