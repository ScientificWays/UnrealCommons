// Scientific Ways

#include "Gameplay/Handhelds/ScWHandheldData_Melee.h"

#include "Gameplay/ScWDamageType.h"
#include "Gameplay/Handhelds/ScWHandheld_Melee.h"
#include "Gameplay/Handhelds/ScWHandheldFragment.h"

const FScWMeleeSwingVariantData FScWMeleeSwingVariantData::Invalid = FScWMeleeSwingVariantData();

UScWHandheldData_Melee::UScWHandheldData_Melee()
{
	DisplayName = FText::FromString(TEXT("Common Melee"));

	bIsUsingCollisionComponent = false;
	CapsuleRadiusHeight = FVector2D(14.0f, 45.0f);
	CapsuleRelativeTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, -20.0f), FVector::OneVector);

	bPushOwnerDuringSwing = false;
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

//~ Begin Swing
float UScWHandheldData_Melee::BP_GetSwingBaseDamageForHandheld_Implementation(const AScWHandheld_Melee* InHandheld) const
{
	float OutDamage = SwingBaseDamage;

	for (const UScWHandheldFragment* SampleFragment : Fragments)
	{
		OutDamage = SampleFragment->BP_ModifySwingBaseDamage(InHandheld, OutDamage);
	}
	return OutDamage;
}

TSubclassOf<UScWDamageType> UScWHandheldData_Melee::BP_GetSwingBaseDamageTypeClassForHandheld_Implementation(const AScWHandheld_Melee* InHandheld) const
{
	TSubclassOf<UScWDamageType> OutDamageTypeClass = SwingBaseDamageTypeClass;

	for (const UScWHandheldFragment* SampleFragment : Fragments)
	{
		OutDamageTypeClass = SampleFragment->BP_ModifySwingBaseDamageTypeClass(InHandheld, OutDamageTypeClass);
	}
	return OutDamageTypeClass;
}
//~ End Swing

//~ Begin Swing Variants
void UScWHandheldData_Melee::BP_GetSwingVariantsForHandheld_Implementation(const AScWHandheld_Melee* InHandheld, TArray<FScWMeleeSwingVariantData>& InOutVariants) const
{
	InOutVariants = SwingVariants;
	ensureReturn(InHandheld);

	for (const UScWHandheldFragment* SampleFragment : Fragments)
	{
		SampleFragment->BP_ModifySwingVariants(InHandheld, InOutVariants);
	}
}
//~ End Swing Variants

//~ Begin Swing FX
UNiagaraComponent* UScWHandheldData_Melee::BP_InitializeSwingParticles_Implementation(AScWHandheld_Melee* InMeleeHandheld) const
{
	if (DefaultSwingParticles)
	{
		ensureReturn(InMeleeHandheld, nullptr);
		USceneComponent* AttachToComponent = InMeleeHandheld->GetRelevantMeshComponent();

		AScWCharacter* OwnerCharacter = InMeleeHandheld->GetOwnerCharacter();
		ensureReturn(OwnerCharacter, nullptr);

		if (AttachToComponent == nullptr)
		{
			AttachToComponent = OwnerCharacter->GetMesh();
		}
		FFXSystemSpawnParameters SpawnParams;
		SpawnParams.WorldContextObject = this;
		SpawnParams.SystemTemplate = DefaultSwingParticles;
		SpawnParams.Location = SwingParticlesRelativeTransform.GetLocation();
		SpawnParams.Rotation = SwingParticlesRelativeTransform.GetRotation().Rotator();
		SpawnParams.Scale = SwingParticlesRelativeTransform.GetScale3D();
		SpawnParams.AttachToComponent = AttachToComponent;
		SpawnParams.AttachPointName = SwingParticlesAttachmentSocketName;
		SpawnParams.LocationType = EAttachLocation::KeepRelativeOffset;
		SpawnParams.bAutoDestroy = true;
		SpawnParams.bAutoActivate = true;
		SpawnParams.PoolingMethod = EPSCPoolMethod::None;
		SpawnParams.bPreCullCheck = true;
		SpawnParams.bIsPlayerEffect = OwnerCharacter->IsPlayerCharacter();
		return UNiagaraFunctionLibrary::SpawnSystemAttachedWithParams(SpawnParams);
	}
	return nullptr;
}
//~ End Swing FX
