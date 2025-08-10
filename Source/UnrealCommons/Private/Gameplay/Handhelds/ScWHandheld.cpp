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


}

//~ Begin Static
AScWHandheld* AScWHandheld::SpawnHandheldFor(AScWCharacter* InOwner, UScWHandheldData* InData)
{
	ensureReturn(InOwner, nullptr);
	ensureReturn(InData, nullptr);
	
	UWorld* World = InOwner->GetWorld();
	ensureReturn(World, nullptr);
	
	FTransform SpawnTransform = InOwner->GetActorTransform();

	ensureReturn(InData->HandheldClass, nullptr);
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

	if (StaticMeshComponent)
	{
		StaticMeshComponent->DestroyComponent();
		StaticMeshComponent = nullptr;
	}
	if (SkeletalMeshComponent)
	{
		SkeletalMeshComponent->DestroyComponent();
		SkeletalMeshComponent = nullptr;
	}
	if (DataAsset->IsUsingStaticMesh())
	{
		StaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
		ensureReturn(StaticMeshComponent);

		StaticMeshComponent->SetStaticMesh(DataAsset->TryGetStaticMesh());
		StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
		StaticMeshComponent->bCastDynamicShadow = true;
		StaticMeshComponent->bAffectDynamicIndirectLighting = true;
		StaticMeshComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		StaticMeshComponent->SetupAttachment(RootComponent);
		StaticMeshComponent->RegisterComponent();
	}
	else if (DataAsset->IsUsingSkeletalMesh())
	{
		SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass());
		ensureReturn(SkeletalMeshComponent);

		SkeletalMeshComponent->SetSkeletalMeshAsset(DataAsset->TryGetSkeletalMesh());
		SkeletalMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
		SkeletalMeshComponent->bCastDynamicShadow = true;
		SkeletalMeshComponent->bAffectDynamicIndirectLighting = true;
		SkeletalMeshComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		SkeletalMeshComponent->SetupAttachment(RootComponent);
		SkeletalMeshComponent->RegisterComponent();

		SkeletalMeshComponent->SetAnimInstanceClass(DataAsset->HandheldAnimInstanceClass);

		UScWAnimInstance_Handheld* HandheldAnimInstance = Cast<UScWAnimInstance_Handheld>(SkeletalMeshComponent->GetAnimInstance());
		ensureReturn(HandheldAnimInstance);

		HandheldAnimInstance->SetCurrentStaticAnimationData(DataAsset->HandheldStaticAnimationData);
	}
	else
	{
		//ensure(false && TEXT("DataAsset does not have a valid StaticMesh or SkeletalMesh!"));
	}
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
		if (DataAsset->IsUsingSkeletalMesh())
		{
			ensureReturn(SkeletalMeshComponent);
			SkeletalMeshComponent->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);
		}
	}
	else
	{
		UScWCharacterMesh_ThirdPerson* OwnerThirdPersonMesh = OwnerCharacter->GetScWThirdPersonCharacterMesh();
		ensureIf(OwnerThirdPersonMesh)
		{
			AttachToComponent(OwnerThirdPersonMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, DataAsset->TP_OwnerMeshAttachmentSocketName);
		}
		if (DataAsset->IsUsingSkeletalMesh())
		{
			ensureReturn(SkeletalMeshComponent);
			SkeletalMeshComponent->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::WorldSpaceRepresentation);
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

	if (DataAsset->IsUsingSkeletalMesh())
	{
		ensureReturn(SkeletalMeshComponent);
		ensureReturn(SkeletalMeshComponent->GetSkeletalMeshAsset());

		SkeletalMeshComponent->SetSimulatePhysics(true);
		SkeletalMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));

		SkeletalMeshComponent->AddImpulse(OwnerCharacter->GetVelocity(), NAME_None, true);
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

UMeshComponent* AScWHandheld::GetRelevantMeshComponent() const
{
	ensureReturn(DataAsset, nullptr);
	if (DataAsset->IsUsingStaticMesh())
	{
		return StaticMeshComponent;
	}
	else if (DataAsset->IsUsingSkeletalMesh())
	{
		return SkeletalMeshComponent;
	}
	else
	{
		return nullptr;
	}
}

UScWAnimInstance_Handheld* AScWHandheld::GetMeshAnimInstance() const
{
	ensureReturn(SkeletalMeshComponent, nullptr);
	return Cast<UScWAnimInstance_Handheld>(SkeletalMeshComponent->GetAnimInstance());
}
//~ End Components
