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
