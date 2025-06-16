// G.o.o.P Studios

#include "GAS/ATAWeapon_Base.h"

#include "Characters/ATACharacter.h"

#include "GAS/DataAssets/WeaponData_Base.h"

AATAWeapon_Base::AATAWeapon_Base(const FObjectInitializer& InObjectInitializer)
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
AATAWeapon_Base* AATAWeapon_Base::SpawnWeapon(AATACharacter* InOwner, UWeaponData_Base* InData)
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
	AATAWeapon_Base* OutWeapon = World->SpawnActorDeferred<AATAWeapon_Base>(InData->WeaponClass, SpawnTransform, InOwner, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
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
void AATAWeapon_Base::K2_UpdateFromDataAsset_Implementation()
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

void AATAWeapon_Base::OnConstruction(const FTransform& InTransform) // AActor
{
	Super::OnConstruction(InTransform);

	K2_UpdateFromDataAsset();
}

void AATAWeapon_Base::BeginPlay() // AActor
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
