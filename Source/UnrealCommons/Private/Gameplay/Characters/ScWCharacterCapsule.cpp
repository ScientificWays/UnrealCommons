// Scientific Ways

#include "Gameplay/Characters/ScWCharacterCapsule.h"

#include "Gameplay/Characters/ScWCharacterData.h"

#include "Gameplay/ScWASC_Base.h"

UScWCharacterCapsule::UScWCharacterCapsule()
{
    
}

//~ Begin Initialize
void UScWCharacterCapsule::InitFromASC(UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) // IScWASC_InitInterface
{
	ensureReturn(InInitASC);
	//ensureReturn(InOwnerActor);
	//ensureReturn(InAvatarActor);

	InInitASC->OnDamageApplied.AddDynamic(this, &ThisClass::HandleDamageApplied);
	InInitASC->OnDied.AddDynamic(this, &ThisClass::HandleDied);
}

void UScWCharacterCapsule::InitFromCharacterData(const UScWCharacterData* InInitCharacterData) // IScWCharacterData_InitInterface
{
	ensureReturn(InInitCharacterData);

	SetCapsuleSize(InInitCharacterData->CapsuleRadiusHeight.X, InInitCharacterData->CapsuleRadiusHeight.Y);
}
//~ End Initialize

//~ Begin Damage
void UScWCharacterCapsule::HandleDamageApplied(float InDamage, const FReceivedDamageData& InData)
{
	UScWASC_Base* InitASC = GetInitASC();
	ensureReturn(InitASC);

	
}

void UScWCharacterCapsule::HandleDied()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCanEverAffectNavigation(false);
	Deactivate();
}
//~ End Damage
