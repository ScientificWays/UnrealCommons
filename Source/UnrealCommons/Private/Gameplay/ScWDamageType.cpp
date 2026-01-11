// Scientific Ways

#include "Gameplay/ScWDamageType.h"

#include "Gameplay/ScWASC_Base.h"
#include "Gameplay/Combo/ScWComboData.h"
#include "Gameplay/ScWASC_InitInterface.h"
#include "Gameplay/Characters/ScWCharacter.h"
#include "Gameplay/Characters/ScWCharacterMovement.h"

UScWDamageType::UScWDamageType()
{
	DisplayName = FText::FromString(TEXT("[Please set DisplayName]"));
	AnalyticsName = TEXT("[Please set AnalyticsName]");

	ApplyDamageGameplayEffectDuration = 0.05f;

	bApplyImpactImpulse = true;
	ImpactImpulsePerDamagePoint = 20.0f;
	ImpactImpulseMassFactor = 0.2f;
}

//~ Begin Impact
void UScWDamageType::ApplyImpactImpulseToCharacterMovement(UScWCharacterMovement* InCharacterMovement, const FHitResult& InHitResult) const
{
	ensureReturn(InCharacterMovement);
	InCharacterMovement->Launch(BP_GetImpactImpulseForHit(InCharacterMovement, InHitResult));
}

void UScWDamageType::ApplyImpactImpulseToPhysicsBody(UPrimitiveComponent* InBody, const FHitResult& InHitResult) const
{
	ensureReturn(InBody);
	InBody->AddImpulseAtLocation(BP_GetImpactImpulseForHit(InBody, InHitResult), InHitResult.ImpactPoint, InHitResult.BoneName);
}

FVector UScWDamageType::BP_GetImpactImpulseForHit_Implementation(const UActorComponent* InTargetComponent, const FHitResult& InHitResult) const
{
	float OutImpulse = DamageImpulse;

	if (const IScWASC_InitInterface* TargetASCInitInterface = Cast<IScWASC_InitInterface>(InTargetComponent))
	{
		if (const UScWASC_Base* TargetASC = TargetASCInitInterface->GetInitASC())
		{
			OutImpulse = TargetASC->GetLastAppliedDamage() * ImpactImpulsePerDamagePoint;

			const FReceivedDamageData& LastAppliedDamageData = TargetASC->GetLastAppliedDamageData();
			if (const UScWASC_Base* AttackerASC = LastAppliedDamageData.TryGetAttackerBaseASC())
			{
				if (const UScWComboData* AttackerRelevantCombo = AttackerASC->GetRelevantCombo())
				{
					OutImpulse *= AttackerRelevantCombo->DamageImpactImpulseMul;
				}
			}
		}
	}
	if (const UCharacterMovementComponent* TargetCharacterMovement = Cast<UCharacterMovementComponent>(InTargetComponent))
	{
		if (ImpactImpulseMassFactor > 0.0f)
		{
			OutImpulse /= FMath::Max(FMath::Lerp(1.0f, TargetCharacterMovement->Mass, ImpactImpulseMassFactor), 0.001f);
		}
	}
	return -InHitResult.ImpactNormal * OutImpulse;
}
//~ End Impact
