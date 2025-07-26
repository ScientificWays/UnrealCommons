// Scientific Ways

#include "Gameplay/Handhelds/ScWHandheldData_Melee.h"

#include "Gameplay/ScWDamageType.h"
#include "Gameplay/Handhelds/ScWHandheld_Melee.h"

const FScWMeleeSwingVariantData FScWMeleeSwingVariantData::Invalid = FScWMeleeSwingVariantData();

UScWHandheldData_Melee::UScWHandheldData_Melee()
{
	DisplayName = FText::FromString(TEXT("Common Melee"));

	bIsUsingCollisionComponent = false;
	CapsuleRadiusHeight = FVector2D(14.0f, 45.0f);
	CapsuleRelativeTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, -20.0f), FVector::OneVector);

	SwingBaseDamage = 10.0f;
	SwingBaseDamageTypeClass = UScWDamageType::StaticClass();

	PostSwingComboTimeWindow = 0.4f;
	SwingAIMaxRange = 128.0f;
	SwingAIMaxRange_BlackboardKeyName = TEXT("MeleeRangeMax");

	VariantBaseDuration = 1.0f;
}

//~ Begin Variants
int32 UScWHandheldData_Melee::BP_GetNewSwingVariantIndexFor_Implementation(const AScWHandheld_Melee* InMeleeHandheld) const
{
	ensureReturn(InMeleeHandheld, INDEX_NONE);
	int32 SwingCounter = InMeleeHandheld->GetSwingCounter();
	
	ensureReturn(!Variants.IsEmpty(), INDEX_NONE);
	return SwingCounter % Variants.Num();
}

float UScWHandheldData_Melee::GetNextPatternDelayTime(int32 InVariantIndex, int32 InNextPatternIndex) const
{
	ensureReturn(Variants.IsValidIndex(InVariantIndex), 0.0f);

	const auto& TracePatterns = Variants[InVariantIndex].TracePatterns;
	return (TracePatterns.Num() < 2) ? (0.0f) : (VariantBaseDuration / (float)TracePatterns.Num());
}
//~ End Variants
