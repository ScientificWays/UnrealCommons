// Scientific Ways

#include "Gameplay/ScWDamageType.h"

#include "Gameplay/Characters/ScWCharacterMovement.h"

UScWDamageType::UScWDamageType()
{
	
}

//~ Begin Impact
void UScWDamageType::ApplyImpactImpulseToCharacterMovement(UScWCharacterMovement* InCharacterMovement, const FHitResult& InHitResult) const
{
	ensureReturn(InCharacterMovement);
	InCharacterMovement->Launch(BP_GetImpactImpulseForHit(InHitResult));
}

void UScWDamageType::ApplyImpactImpulseToPhysicsBody(UPrimitiveComponent* InBody, const FHitResult& InHitResult) const
{
	ensureReturn(InBody);
	InBody->AddImpulseAtLocation(BP_GetImpactImpulseForHit(InHitResult), InHitResult.ImpactPoint, InHitResult.BoneName);
}

FVector UScWDamageType::BP_GetImpactImpulseForHit_Implementation(const FHitResult& InHitResult) const
{
	return DamageImpulse * -InHitResult.ImpactNormal;
}
//~ End Impact
