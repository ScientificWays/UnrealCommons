// Scientific Ways

#include "Gameplay/Weapons/ScWWeapon_Melee.h"

#include "Gameplay/Characters/ScWCharacter.h"

#include "Gameplay/ScWGameplayFunctionLibrary.h"
#include "Gameplay/Weapons/ScWWeaponData_Melee.h"

#include "World/ScWTypes_World.h"

AScWWeapon_Melee::AScWWeapon_Melee(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	CollisionComponent->SetCollisionProfileName(TEXT("MeleeWeapon"), false);
	CollisionComponent->SetupAttachment(RootComponent);
}

//~ Begin Initialize
UScWWeaponData_Melee* AScWWeapon_Melee::GetMeleeDataAsset() const
{
	return Cast<UScWWeaponData_Melee>(DataAsset);
}

void AScWWeapon_Melee::BP_UpdateFromDataAsset_Implementation() // AScWWeapon_Base
{
	Super::BP_UpdateFromDataAsset_Implementation();

	UScWWeaponData_Melee* WeaponDataAsset = GetMeleeDataAsset();
	ensureReturn(WeaponDataAsset);

	if (CollisionComponent)
	{
		if (WeaponDataAsset->bIsUsingPatterns)
		{
			CollisionComponent->SetAutoActivate(false);
			CollisionComponent->SetActive(false);
		}
		else
		{
			CollisionComponent->SetAutoActivate(true);
			CollisionComponent->SetActive(true);
		}
		CollisionComponent->SetCapsuleSize(WeaponDataAsset->CapsuleRadiusHeight.X, WeaponDataAsset->CapsuleRadiusHeight.Y);
		CollisionComponent->SetRelativeTransform(WeaponDataAsset->CapsuleRelativeTransform);
	}
}

void AScWWeapon_Melee::OnConstruction(const FTransform& InTransform) // AActor
{
	Super::OnConstruction(InTransform);

	if (UWorld* World = GetWorld())
	{
		if (World->IsEditorWorld() && DataAsset)
		{
			BP_UpdateFromDataAsset();
		}
	}
}

void AScWWeapon_Melee::BeginPlay() // AActor
{
	UScWWeaponData_Melee* WeaponDataAsset = GetMeleeDataAsset();
	ensureReturn(WeaponDataAsset);

	if (CollisionComponent)
	{
		if (WeaponDataAsset->bIsUsingPatterns)
		{
			
		}
		else
		{
			CollisionComponent->ClearMoveIgnoreActors();
			CollisionComponent->IgnoreActorWhenMoving(OwnerCharacter, true);

			CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AScWWeapon_Melee::OnCollisionComponentBeginOverlap);
		}
	}
	BP_EndSwing();

	DefaultPatternTraceIgnoredActorArray = { OwnerCharacter, this };

	Super::BeginPlay();
}
//~ End Initialize

//~ Begin Owner
void AScWWeapon_Melee::HandleDrop() // AScWWeapon_Base
{
	Super::HandleDrop();

	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
//~ End Owner

//~ Begin Components
void AScWWeapon_Melee::OnCollisionComponentBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepHitResult)
{
	if (bInFromSweep)
	{
		BP_HandleSwingHit(InSweepHitResult);
	}
	else
	{
		FVector ThisLocation = InOverlappedComponent ? InOverlappedComponent->GetComponentLocation() : FVector::ZeroVector;
		FVector TargetLocation = InOtherComponent ? InOtherComponent->GetComponentLocation() : FVector::ZeroVector;

		FHitResult MinimalInfoHitResult = FHitResult(InOtherActor, InOtherComponent, TargetLocation, (TargetLocation - ThisLocation).GetSafeNormal());
		MinimalInfoHitResult.TraceStart = ThisLocation;
		MinimalInfoHitResult.TraceEnd = TargetLocation;
		BP_HandleSwingHit(MinimalInfoHitResult);
	}
}
//~ End Components

//~ Begin Swing
void AScWWeapon_Melee::BP_BeginSwing_Implementation(float InSwingDamage, TSubclassOf<UDamageType> InSwingDamageTypeClass)
{
	LastSwingDamage = InSwingDamage;
	LastSwingDamageTypeClass = InSwingDamageTypeClass;
	LastSwingAffectedActorArray.Empty();

	UScWWeaponData_Melee* WeaponDataAsset = GetMeleeDataAsset();
	ensureReturn(WeaponDataAsset);

	if (WeaponDataAsset->bIsUsingPatterns)
	{
		BP_BeginPatternTraces();
	}
	else
	{
		ensureReturn(CollisionComponent);
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AScWWeapon_Melee::BP_EndSwing_Implementation()
{
	UScWWeaponData_Melee* WeaponDataAsset = GetMeleeDataAsset();
	ensureReturn(WeaponDataAsset);

	if (WeaponDataAsset->bIsUsingPatterns)
	{

	}
	else
	{
		ensureReturn(CollisionComponent);
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AScWWeapon_Melee::BP_HandleSwingHit_Implementation(const FHitResult& InHitResult)
{
	ensureReturn(OwnerCharacter);

	AActor* HitActor = InHitResult.GetActor();
	ensureReturn(HitActor);

	if (LastSwingAffectedActorArray.Contains(HitActor))
	{

	}
	else
	{
		LastSwingAffectedActorArray.Add(HitActor);

		FVector HitDirection = (InHitResult.TraceEnd - InHitResult.TraceStart).GetSafeNormal();
		UScWGameplayFunctionLibrary::ApplyPointDamage(this, HitActor, LastSwingDamage, HitDirection, InHitResult, OwnerCharacter->GetController(), LastSwingDamageTypeClass);
	}
}
//~ End Swing

//~ Begin Patterns
FVector AScWWeapon_Melee::BP_GetPatternStartLocation_Implementation(const FScWMeleeSwingPatternData& InPatternData, int32 InPatternIndex) const
{
	ensureReturn(OwnerCharacter, GetActorLocation());
	return OwnerCharacter->GetPawnViewLocation() + OwnerCharacter->GetViewRotation().Vector() * InPatternData.TraceOffsetLocation;
}

void AScWWeapon_Melee::BP_BeginPatternTraces_Implementation()
{
	UScWWeaponData_Melee* WeaponDataAsset = GetMeleeDataAsset();
	ensureReturn(WeaponDataAsset);

	ensureReturn(!WeaponDataAsset->Patterns.IsEmpty());
	BP_HandlePatternTrace(WeaponDataAsset->Patterns[0], 0);
}

void AScWWeapon_Melee::BP_HandlePatternTrace_Implementation(const FScWMeleeSwingPatternData& InPatternData, int32 InPatternIndex)
{
	ensureReturn(OwnerCharacter);

	UScWWeaponData_Melee* WeaponDataAsset = GetMeleeDataAsset();
	ensureReturn(WeaponDataAsset);

	UWorld* World = GetWorld();
	ensureReturn(World);
	
	FVector TraceStart = BP_GetPatternStartLocation(InPatternData, InPatternIndex);
	FVector TraceEnd = TraceStart + (OwnerCharacter->GetBaseAimRotation() + InPatternData.TraceOffsetRotation).Vector() * InPatternData.TraceLength;

	TArray<FHitResult> TraceHitResults;
	FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Append(DefaultPatternTraceIgnoredActorArray);
	UKismetSystemLibrary::SphereTraceMulti(this, TraceStart, TraceEnd, InPatternData.TraceShapeRadius, TraceTypeQuery_Melee, false, ActorsToIgnore, PatternTraceDebugType, TraceHitResults, true);
	
	if (TraceHitResults.IsEmpty())
	{
		return;
	}
	for (const FHitResult& SampleHitResult : TraceHitResults)
	{
		BP_HandleSwingHit(SampleHitResult);
	}
	int32 NextPatternIndex = InPatternIndex + 1;
	if (WeaponDataAsset->Patterns.IsValidIndex(NextPatternIndex))
	{
		float NextPatternDelayTime = WeaponDataAsset->GetNextPatternDelayTime(NextPatternIndex);
		if (NextPatternDelayTime > 0.0f)
		{
			FTimerDelegate NextPatternMethodDelegate;
			NextPatternMethodDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED_TwoParams(AScWWeapon_Melee, BP_HandlePatternTrace, const FScWMeleeSwingPatternData&, int32), InPatternData, NextPatternIndex);
			World->GetTimerManager().SetTimer(NextPatternDelayHandle, NextPatternMethodDelegate, NextPatternDelayTime, false);
		}
		else
		{
			BP_HandlePatternTrace(InPatternData, NextPatternIndex);
		}
	}
}
//~ End Patterns

