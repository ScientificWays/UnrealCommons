// Scientific Ways

#include "Gameplay/Characters/ScWCharacterData.h"

#include "AI/ScWAIPC_Base.h"
#include "AI/ScWAIController.h"

#include "Gameplay/Characters/ScWCharacter.h"
#include "Gameplay/Handhelds/ScWHandheld_Melee.h"
#include "Gameplay/Characters/ScWCharacterData_InitInterface.h"

#include "Framework/ScWGameState.h"

#include "Perception/AISenseConfig_Sight.h"

UScWCharacterData::UScWCharacterData()
{
	DisplayName = FText::FromString(TEXT("Unnamed Character"));

	DefaultTeamName = TEXT("NoTeam");

	TP_SkeletalMeshRelativeTransform = FTransform(FRotator(0.0f, -90.0, 0.0f), FVector(0.0f, 0.0f, -90.0f), FVector::OneVector);
	bRagdollOnDeath = true;

	CapsuleRadiusHeight = FVector2D(34.0f, 90.0f);

	CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, SpawnEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonCharacterInit");

	DamageImpactParticlesColor = FColor::Red;

	SightRadius = 2000.0f;
	LoseSightRadiusOffset = 100.0f;
}

//~ Begin Initialize
void UScWCharacterData::BP_InitializeCharacterComponents_Implementation(AScWCharacter* InCharacter) const
{
	ensureReturn(InCharacter);
	IScWCharacterData_InitInterface::HandleInit(InCharacter, this);
}

void UScWCharacterData::BP_InitializeCharacterController_Implementation(AScWCharacter* InCharacter) const
{
	ensureReturn(InCharacter);
	InCharacter->SetGenericTeamId(GetDefaultTeamId(InCharacter));

	if (AIControllerClass)
	{
		InCharacter->AIControllerClass = AIControllerClass;
	}
	if (AScWAIController* CharacterAIController = InCharacter->GetController<AScWAIController>())
	{
		ensureReturn(DefaultBehaviorTree);
		CharacterAIController->RunBehaviorTree(DefaultBehaviorTree);

		UAISenseConfig_Sight* SightConfig = CharacterAIController->GetBaseAIPC()->GetSenseConfig<UAISenseConfig_Sight>();
		ensureReturn(SightConfig);
		SightConfig->SightRadius = SightRadius;
		SightConfig->LoseSightRadius = SightRadius + LoseSightRadiusOffset;
	}
}
//~ End Initialize

//~ Begin Teams
FGenericTeamId UScWCharacterData::GetDefaultTeamId(const UObject* InWCO) const
{
	AScWGameState* BaseGameState = AScWGameState::TryGetScWGameState(InWCO);
	ensureReturn(BaseGameState, FGenericTeamId::NoTeam);
	return BaseGameState->GetTeamId(DefaultTeamName);
}
//~ End Teams

//~ Begin FX
UNiagaraComponent* UScWCharacterData::BP_SpawnDamageImpactParticlesFromGameplayCue_Implementation(const UObject* InWCO, const FGameplayCueParameters& InParams) const
{
	static const FName DamageParameterName = TEXT("Damage");
	static const FName BloodColorParameterName = TEXT("BloodColor");
	static const FName GlowVariantParameterName = TEXT("GlowVariant");

	UNiagaraComponent* OutParticles = UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWCO, DamageImpactParticles, InParams.Location, InParams.Normal.Rotation(), FVector::OneVector, true, true, ENCPoolMethod::AutoRelease);
	ensureReturn(OutParticles, nullptr);

	OutParticles->SetVariableFloat(DamageParameterName, InParams.RawMagnitude);
	OutParticles->SetVariableLinearColor(BloodColorParameterName, DamageImpactParticlesColor);

	int32 GlowVariant = 0;

	if (AScWHandheld_Melee* SourceMeleeHandheld = Cast<AScWHandheld_Melee>(InParams.GetEffectCauser()))
	{
		GlowVariant = SourceMeleeHandheld->GetSwingCounter() % 2;
	}
	else
	{
		GlowVariant = FMath::Rand32() % 2;
	}
	OutParticles->SetVariableInt(GlowVariantParameterName, GlowVariant);
	return OutParticles;
}
//~ End FX
