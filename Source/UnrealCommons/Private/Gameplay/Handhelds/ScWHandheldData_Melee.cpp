// Scientific Ways

#include "Gameplay/Handhelds/ScWHandheldData_Melee.h"

#include "Gameplay/ScWDamageType.h"

UScWHandheldData_Melee::UScWHandheldData_Melee()
{
	DisplayName = FText::FromString(TEXT("Common Melee"));

	CapsuleRadiusHeight = FVector2D(14.0f, 45.0f);
	CapsuleRelativeTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, -20.0f), FVector::OneVector);

	SwingBaseDamage = 10.0f;
	SwingBaseDamageTypeClass = UScWDamageType::StaticClass();

	PostSwingComboTimeWindow = 0.4f;
	SwingAIMaxRange = 128.0f;
	SwingAIMaxRange_BlackboardKeyName = TEXT("MeleeRangeMax");

	bIsUsingPatterns = false;
	PatternsTriggerTime = 1.0f;
}

//~ Begin Patterns
float UScWHandheldData_Melee::GetNextPatternDelayTime(int32 InNextPatternIndex) const
{
	return (Patterns.Num() < 2) ? (0.0f) : (PatternsTriggerTime / (float)Patterns.Num());
}
//~ End Patterns
