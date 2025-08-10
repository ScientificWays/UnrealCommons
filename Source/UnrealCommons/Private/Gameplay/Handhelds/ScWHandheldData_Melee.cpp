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

	bPushPlayerDuringSwing = false;
	SwingPushVector = FVector(400.0f, 0.0f, 0.0f);
	SwingPushDuration = 0.2f;

	SwingBaseDamage = 10.0f;
	SwingBaseDamageTypeClass = UScWDamageType::StaticClass();
	bSwingOwnerEffectOnlyDuringSwing = true;

	PostSwingComboTimeWindow = 0.4f;
	SwingAIMaxRange = 128.0f;
	SwingAIMaxRange_BlackboardKeyName = TEXT("MeleeRangeMax");

	SwingVariantBaseDuration = 1.0f;

	SwingParticlesAttachmentSocketName = NAME_None;
	SwingParticlesRelativeTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, 0.0f), FVector::OneVector);
}
