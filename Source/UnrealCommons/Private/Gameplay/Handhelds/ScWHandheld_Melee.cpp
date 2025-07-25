// Scientific Ways

#include "Gameplay/Handhelds/ScWHandheld_Melee.h"

#include "Gameplay/Characters/ScWCharacter.h"

#include "Gameplay/ScWGameplayFunctionLibrary.h"
#include "Gameplay/Handhelds/ScWHandheldData_Melee.h"

AScWHandheld_Melee::AScWHandheld_Melee(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	CollisionComponent->SetCollisionProfileName(TEXT("MeleeWeapon"), false);
	CollisionComponent->SetupAttachment(RootComponent);
}

//~ Begin Initialize
UScWHandheldData_Melee* AScWHandheld_Melee::GetMeleeDataAsset() const
{
	return Cast<UScWHandheldData_Melee>(DataAsset);
}

void AScWHandheld_Melee::BP_UpdateFromDataAsset_Implementation() // AScWHandheld
{
	Super::BP_UpdateFromDataAsset_Implementation();

	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset);

	if (CollisionComponent)
	{
		if (MeleeDataAsset->bIsUsingPatterns)
		{
			CollisionComponent->SetAutoActivate(false);
			CollisionComponent->SetActive(false);
		}
		else
		{
			CollisionComponent->SetAutoActivate(true);
			CollisionComponent->SetActive(true);
		}
		CollisionComponent->SetCapsuleSize(MeleeDataAsset->CapsuleRadiusHeight.X, MeleeDataAsset->CapsuleRadiusHeight.Y);
		CollisionComponent->SetRelativeTransform(MeleeDataAsset->CapsuleRelativeTransform);
	}
}

void AScWHandheld_Melee::OnConstruction(const FTransform& InTransform) // AActor
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

void AScWHandheld_Melee::BeginPlay() // AActor
{
	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset);

	if (CollisionComponent)
	{
		if (MeleeDataAsset->bIsUsingPatterns)
		{
			
		}
		else
		{
			CollisionComponent->ClearMoveIgnoreActors();
			CollisionComponent->IgnoreActorWhenMoving(OwnerCharacter, true);

			CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AScWHandheld_Melee::OnCollisionComponentBeginOverlap);
		}
	}
	SwingCounter = 0;

	BP_EndSwing();

	DefaultPatternTraceIgnoredActorArray = { OwnerCharacter, this };

	Super::BeginPlay();
}
//~ End Initialize

//~ Begin Owner
void AScWHandheld_Melee::HandleDrop() // AScWHandheld
{
	Super::HandleDrop();

	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
//~ End Owner

//~ Begin Components
void AScWHandheld_Melee::OnCollisionComponentBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepHitResult)
{
	if (bInFromSweep)
	{
		BP_HandleSwingHit(InSweepHitResult);
	}
	else
	{
		FHitResult MinimalInfoHitResult;
		UScWGameplayFunctionLibrary::MakeMinimalInfoDamageImpactHitResult(this, InOverlappedComponent, OwnerCharacter, InOtherActor, InOtherComponent, MinimalInfoHitResult);
		BP_HandleSwingHit(MinimalInfoHitResult);
	}
}
//~ End Components

//~ Begin Swing
void AScWHandheld_Melee::BP_BeginSwing_Implementation(float InSwingDamage, TSubclassOf<UDamageType> InSwingDamageTypeClass)
{
	++SwingCounter;

	LastSwingDamage = InSwingDamage;
	LastSwingDamageTypeClass = InSwingDamageTypeClass;
	LastSwingAffectedActorArray.Empty();

	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset);

	if (MeleeDataAsset->bIsUsingPatterns)
	{
		BP_BeginPatternTraces();
	}
	else
	{
		ensureReturn(CollisionComponent);
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AScWHandheld_Melee::BP_EndSwing_Implementation()
{
	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset);

	if (MeleeDataAsset->bIsUsingPatterns)
	{

	}
	else
	{
		ensureReturn(CollisionComponent);
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AScWHandheld_Melee::BP_HandleSwingHit_Implementation(const FHitResult& InHitResult)
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
FVector AScWHandheld_Melee::BP_GetPatternStartLocation_Implementation(const FScWMeleeSwingPatternData& InPatternData, int32 InPatternIndex) const
{
	ensureReturn(OwnerCharacter, GetActorLocation());
	return OwnerCharacter->GetPawnViewLocation() + OwnerCharacter->GetViewRotation().Vector() * InPatternData.TraceOffsetLocation;
}

void AScWHandheld_Melee::BP_BeginPatternTraces_Implementation()
{
	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset);

	ensureReturn(!MeleeDataAsset->Patterns.IsEmpty());
	BP_HandlePatternTrace(MeleeDataAsset->Patterns[0], 0);
}

void AScWHandheld_Melee::BP_HandlePatternTrace_Implementation(const FScWMeleeSwingPatternData& InPatternData, int32 InPatternIndex)
{
	ensureReturn(OwnerCharacter);

	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset);

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
	if (MeleeDataAsset->Patterns.IsValidIndex(NextPatternIndex))
	{
		float NextPatternDelayTime = MeleeDataAsset->GetNextPatternDelayTime(NextPatternIndex);
		if (NextPatternDelayTime > 0.0f)
		{
			FTimerDelegate NextPatternMethodDelegate;
			NextPatternMethodDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED_TwoParams(AScWHandheld_Melee, BP_HandlePatternTrace, const FScWMeleeSwingPatternData&, int32), InPatternData, NextPatternIndex);
			World->GetTimerManager().SetTimer(NextPatternDelayHandle, NextPatternMethodDelegate, NextPatternDelayTime, false);
		}
		else
		{
			BP_HandlePatternTrace(InPatternData, NextPatternIndex);
		}
	}
}
//~ End Patterns

