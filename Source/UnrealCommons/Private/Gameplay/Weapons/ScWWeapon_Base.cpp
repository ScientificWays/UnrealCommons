// Scientific Ways

#include "Gameplay/Weapons/ScWWeapon_Base.h"

#include "Gameplay/Characters/ScWCharacter.h"

#include "Gameplay/Weapons/ScWWeaponData_Base.h"

AScWWeapon_Base::AScWWeapon_Base(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->bCastDynamicShadow = true;
	Mesh->bAffectDynamicIndirectLighting = true;
	Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Mesh->SetupAttachment(RootComponent);
}

//~ Begin Static
AScWWeapon_Base* AScWWeapon_Base::SpawnWeapon(AScWCharacter* InOwner, UScWWeaponData_Base* InData)
{
	if (!InOwner || !InData)
	{
		return nullptr;
	}
	UWorld* World = InOwner->GetWorld();
	if (!World)
	{
		return nullptr;
	}
	FTransform SpawnTransform = InOwner->GetActorTransform();
	AScWWeapon_Base* OutWeapon = World->SpawnActorDeferred<AScWWeapon_Base>(InData->WeaponClass, SpawnTransform, InOwner, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (OutWeapon)
	{
		OutWeapon->DataAsset = InData;
		OutWeapon->OwnerCharacter = InOwner;
		OutWeapon->FinishSpawning(SpawnTransform);
	}
	return OutWeapon;
}
//~ End Static

//~ Begin Initialize
void AScWWeapon_Base::BP_UpdateFromDataAsset_Implementation()
{
	if (!DataAsset)
	{
		return;
	}
	if (Mesh)
	{
		Mesh->SetStaticMesh(DataAsset->StaticMesh);
	}
}

void AScWWeapon_Base::OnConstruction(const FTransform& InTransform) // AActor
{
	Super::OnConstruction(InTransform);

	BP_UpdateFromDataAsset();
}

void AScWWeapon_Base::BeginPlay() // AActor
{
	Super::BeginPlay();

	ensure(DataAsset);
	if (DataAsset)
	{
		if (USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, DataAsset->OwnerMeshAttachmentSocket);
		}
	}
}
//~ End Initialize

//~ Begin Owner
void AScWWeapon_Base::HandleDrop()
{
	ensureReturn(OwnerCharacter);
	ensure(OwnerCharacter->GetWeapon() != this);
	
	//ensure(GetParentComponent() == OwnerCharacter->GetMesh());
	//ensure(IsChildActor());
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (Mesh && Mesh->GetStaticMesh())
	{
		Mesh->SetSimulatePhysics(true);
		Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));

		Mesh->AddImpulse(OwnerCharacter->GetVelocity(), NAME_None, true);
	}
}
//~ End Owner

//~ Begin Components
UAbilitySystemComponent* AScWWeapon_Base::GetAbilitySystemComponent() const // IAbilitySystemInterface
{
	if (OwnerCharacter)
	{
		return OwnerCharacter->GetAbilitySystemComponent();
	}
	return nullptr;
}
//~ End Components
