// Scientific Ways

#include "Gameplay/ScWWeapon_Base.h"

#include "Characters/ScWCharacter.h"

#include "Gameplay/DataAssets/ScWWeaponData_Base.h"

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
void AScWWeapon_Base::K2_UpdateFromDataAsset_Implementation()
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

	K2_UpdateFromDataAsset();
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
