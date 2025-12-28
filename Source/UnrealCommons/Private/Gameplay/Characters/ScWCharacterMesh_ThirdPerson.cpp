// Scientific Ways

#include "Gameplay/Characters/ScWCharacterMesh_ThirdPerson.h"

#include "Animations/ScWAnimInstance_ThirdPerson.h"
#include "Animations/ScWAnimationsFunctionLibrary.h"

#include "Gameplay/ScWASC_Base.h"
#include "Gameplay/ScWDamageType.h"
#include "Gameplay/Handhelds/ScWHandheld.h"
#include "Gameplay/Handhelds/ScWHandheldData.h"
#include "Gameplay/Characters/ScWCharacterData.h"

UScWCharacterMesh_ThirdPerson::UScWCharacterMesh_ThirdPerson()
{
	FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;
	VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	RagdollCollisionProfileName = TEXT("Ragdoll");
}

//~ Begin Initialize
void UScWCharacterMesh_ThirdPerson::BeginPlay() // UActorComponent
{
	Super::BeginPlay();

	
}

void UScWCharacterMesh_ThirdPerson::InitFromCharacterData(const UScWCharacterData* InInitCharacterData) // IScWCharacterData_InitInterface
{
	ensureReturn(InInitCharacterData);

	ensureReturn(InInitCharacterData->TP_SkeletalMesh);
	SetSkeletalMeshAsset(InInitCharacterData->TP_SkeletalMesh);

	SetAnimInstanceClass(InInitCharacterData->TP_AnimInstanceClass);
	SetRelativeTransform(InInitCharacterData->TP_SkeletalMeshRelativeTransform);

	UScWAnimInstance_ThirdPerson* ThirdPersonAnimInstance = Cast<UScWAnimInstance_ThirdPerson>(GetAnimInstance());
	ensureReturn(ThirdPersonAnimInstance);

	ThirdPersonAnimInstance->SetDefaultStaticAnimationData(InInitCharacterData->TP_DefaultStaticAnimationData);
}

void UScWCharacterMesh_ThirdPerson::InitFromASC(UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) // IScWASC_InitInterface
{
	ensureReturn(InInitASC);
	//ensureReturn(InOwnerActor);
	//ensureReturn(InAvatarActor);

	InInitASC->OnDamageApplied.AddDynamic(this, &ThisClass::HandleDamageApplied);
	InInitASC->OnDied.AddDynamic(this, &ThisClass::HandleDied);
}
//~ End Initialize

//~ Begin Updates
void UScWCharacterMesh_ThirdPerson::UpdateFromIsInFirstPersonView(bool bInIsInFirstPersonView)
{
	if (bInIsInFirstPersonView)
	{
		SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::WorldSpaceRepresentation);
	}
	else
	{
		SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::None);
	}
}

void UScWCharacterMesh_ThirdPerson::UpdateFromHandheld(AScWHandheld* InHandheld) // UScWCharacterMesh_Base
{
	UScWAnimInstance_ThirdPerson* ThirdPersonAnimInstance = Cast<UScWAnimInstance_ThirdPerson>(GetAnimInstance());
	ensureReturn(ThirdPersonAnimInstance);

	if (InHandheld)
	{
		UScWHandheldData* HandheldData = InHandheld->GetDataAsset();
		ensureReturn(HandheldData);

		ThirdPersonAnimInstance->Montage_StopGroupByName(0.2f, UScWAnimationsFunctionLibrary::MontageDefaultGroupName);
		ThirdPersonAnimInstance->SetCurrentStaticAnimationData(HandheldData->TP_StaticAnimationData);
	}
	else
	{
		ThirdPersonAnimInstance->ResetToDefaultStaticAnimationData();
	}
}
//~ End Updates

//~ Begin Damage
void UScWCharacterMesh_ThirdPerson::BP_ActivateRagdoll_Implementation()
{
	SetSimulatePhysics(true);
	SetCollisionProfileName(RagdollCollisionProfileName);
}

void UScWCharacterMesh_ThirdPerson::HandleDamageApplied(float InDamage, const FReceivedDamageData& InData)
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

void UScWCharacterMesh_ThirdPerson::HandleDied()
{
	const UScWCharacterData* CharacterData = GetInitCharacterData();
	ensureReturn(CharacterData);

	switch (CharacterData->PostDeathBehavior)
	{
		case EScWCharacterPostDeathBehavior::Ragdoll:
		{
			BP_ActivateRagdoll();
			break;
		}
		case EScWCharacterPostDeathBehavior::AnimationBlueprint:
		{
			SetAnimInstanceClass(CharacterData->TP_DiedAnimInstanceClass);
			break;
		}
	}
}
//~ End Damage
