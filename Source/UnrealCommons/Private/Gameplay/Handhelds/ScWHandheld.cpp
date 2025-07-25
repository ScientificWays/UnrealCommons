// Scientific Ways

#include "Gameplay/Handhelds/ScWHandheld.h"

#include "Animations/ScWAnimInstance_Handheld.h"

#include "Gameplay/Characters/ScWCharacter.h"
#include "Gameplay/Handhelds/ScWHandheldData.h"
#include "Gameplay/Characters/ScWCharacterMesh_FirstPerson.h"
#include "Gameplay/Characters/ScWCharacterMesh_ThirdPerson.h"

AScWHandheld::AScWHandheld(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->bCastDynamicShadow = true;
	Mesh->bAffectDynamicIndirectLighting = true;
	Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Mesh->SetupAttachment(RootComponent);
}

//~ Begin Static
AScWHandheld* AScWHandheld::SpawnHandheldFor(AScWCharacter* InOwner, UScWHandheldData* InData)
{
	ensureReturn(InOwner, nullptr);
	ensureReturn(InData, nullptr);
	
	UWorld* World = InOwner->GetWorld();
	ensureReturn(World, nullptr);
	
	FTransform SpawnTransform = InOwner->GetActorTransform();

	AScWHandheld* OutHandheld = World->SpawnActorDeferred<AScWHandheld>(InData->HandheldClass, SpawnTransform, InOwner, InOwner, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	ensureReturn(OutHandheld, nullptr);
	
	OutHandheld->DataAsset = InData;
	OutHandheld->OwnerCharacter = InOwner;
	OutHandheld->FinishSpawning(SpawnTransform);
	return OutHandheld;
}
//~ End Static

//~ Begin Initialize
void AScWHandheld::BP_UpdateFromDataAsset_Implementation()
{
	ensureReturn(DataAsset);
	ensureReturn(Mesh);
	
	Mesh->SetSkeletalMeshAsset(DataAsset->SkeletalMesh);
	Mesh->SetAnimInstanceClass(DataAsset->HandheldAnimInstanceClass);

	UScWAnimInstance_Handheld* HandheldAnimInstance = Cast<UScWAnimInstance_Handheld>(Mesh->GetAnimInstance());
	ensureReturn(HandheldAnimInstance);

	HandheldAnimInstance->SetCurrentStaticAnimationData(DataAsset->HandheldStaticAnimationData);
}

void AScWHandheld::OnConstruction(const FTransform& InTransform) // AActor
{
	Super::OnConstruction(InTransform);

	BP_UpdateFromDataAsset();
}

void AScWHandheld::BeginPlay() // AActor
{
	Super::BeginPlay();

	UpdateAttachmentToOwner();
}
//~ End Initialize

//~ Begin Owner
void AScWHandheld::UpdateAttachmentToOwner()
{
	ensureReturn(OwnerCharacter);
	ensureReturn(DataAsset);

	if (OwnerCharacter->IsInFirstPersonView())
	{
		UScWCharacterMesh_FirstPerson* OwnerFirstPersonMesh = OwnerCharacter->GetScWFirstPersonCharacterMesh();
		ensureIf(OwnerFirstPersonMesh)
		{
			AttachToComponent(OwnerFirstPersonMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, DataAsset->FP_OwnerMeshAttachmentSocketName);
		}
		ensureIf(Mesh)
		{
			Mesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);
		}
	}
	else
	{
		UScWCharacterMesh_ThirdPerson* OwnerThirdPersonMesh = OwnerCharacter->GetScWThirdPersonCharacterMesh();
		ensureIf(OwnerThirdPersonMesh)
		{
			AttachToComponent(OwnerThirdPersonMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, DataAsset->TP_OwnerMeshAttachmentSocketName);
		}
		ensureIf(Mesh)
		{
			Mesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::WorldSpaceRepresentation);
		}
	}
}

void AScWHandheld::HandleDrop()
{
	ensureReturn(OwnerCharacter);
	ensure(OwnerCharacter->GetHandheld() != this);
	
	//ensure(GetParentComponent() == OwnerCharacter->GetMesh());
	//ensure(IsChildActor());
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	OwnerCharacter = nullptr;

	if (Mesh && Mesh->GetSkeletalMeshAsset())
	{
		Mesh->SetSimulatePhysics(true);
		Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));

		Mesh->AddImpulse(OwnerCharacter->GetVelocity(), NAME_None, true);
	}
}
//~ End Owner

//~ Begin Components
UAbilitySystemComponent* AScWHandheld::GetAbilitySystemComponent() const // IAbilitySystemInterface
{
	if (OwnerCharacter)
	{
		return OwnerCharacter->GetAbilitySystemComponent();
	}
	return nullptr;
}

UScWAnimInstance_Handheld* AScWHandheld::GetMeshAnimInstance() const
{
	ensureReturn(Mesh, nullptr);
	return Cast<UScWAnimInstance_Handheld>(Mesh->GetAnimInstance());
}
//~ End Components
