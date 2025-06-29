// Scientific Ways

#include "Gameplay/Characters/ScWCharacterMesh.h"

#include "Gameplay/Characters/ScWCharacterData.h"

#include "Gameplay/ScWASC_Base.h"
#include "Gameplay/ScWDamageType.h"

UScWCharacterMesh::UScWCharacterMesh()
{
	RagdollCollisionProfileName = TEXT("Ragdoll");
}

//~ Begin Initialize
void UScWCharacterMesh::InitFromASC(UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) // IScWASC_InitInterface
{
	ensureReturn(InInitASC);
	//ensureReturn(InOwnerActor);
	//ensureReturn(InAvatarActor);

	InInitASC->OnDamageApplied.AddDynamic(this, &ThisClass::HandleDamageApplied);
	InInitASC->OnDied.AddDynamic(this, &ThisClass::HandleDied);
}

void UScWCharacterMesh::InitFromCharacterData(const UScWCharacterData* InInitCharacterData) // IScWCharacterData_InitInterface
{
	ensureReturn(InInitCharacterData);

	SetSkeletalMeshAsset(InInitCharacterData->SkeletalMesh);
	SetAnimInstanceClass(InInitCharacterData->AnimInstanceClass);
	SetRelativeTransform(InInitCharacterData->SkeletalMeshRelativeTransform);
}
//~ End Initialize

//~ Begin Damage
void UScWCharacterMesh::HandleDamageApplied(float InDamage, const FReceivedDamageData& InData)
{
	if (IsSimulatingPhysics())
	{
		UScWASC_Base* InitASC = GetInitASC();
		ensureReturn(InitASC);

		const FReceivedDamageData& LastAppliedDamageData = InitASC->GetLastAppliedDamageData();
		ensureReturn(LastAppliedDamageData.DamageType);

		LastAppliedDamageData.DamageType->ApplyImpactImpulseToPhysicsBody(this, LastAppliedDamageData.HitResult);
	}
}

void UScWCharacterMesh::HandleDied()
{
	const UScWCharacterData* InitCharacterData = GetInitCharacterData();
	ensureReturn(InitCharacterData);

	if (InitCharacterData->bRagdollOnDeath)
	{
		SetSimulatePhysics(true);
		SetCollisionProfileName(RagdollCollisionProfileName);
	}
	else if (InitCharacterData->DiedAnimInstanceClass)
	{
		SetAnimInstanceClass(InitCharacterData->DiedAnimInstanceClass);
	}
}
//~ End Damage
