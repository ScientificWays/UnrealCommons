// Scientific Ways

#include "Gameplay/Handhelds/ScWHandheld_Melee.h"

#include "Gameplay/ScWASC_Character.h"
#include "Gameplay/Combo/ScWComboData.h"
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
		if (MeleeDataAsset->bIsUsingCollisionComponent)
		{
			CollisionComponent->SetCapsuleSize(MeleeDataAsset->CapsuleRadiusHeight.X, MeleeDataAsset->CapsuleRadiusHeight.Y);
			CollisionComponent->SetRelativeTransform(MeleeDataAsset->CapsuleRelativeTransform);

			CollisionComponent->SetAutoActivate(true);
			CollisionComponent->SetActive(true);
		}
		else
		{
			CollisionComponent->SetAutoActivate(false);
			CollisionComponent->SetActive(false);
		}
	}
}

void AScWHandheld_Melee::OnConstruction(const FTransform& InTransform) // AActor
{
	Super::OnConstruction(InTransform);

	if (IS_EDITOR_WORLD() && DataAsset)
	{
		BP_UpdateFromDataAsset();
	}
}

void AScWHandheld_Melee::BeginPlay() // AActor
{
	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset);

	if (CollisionComponent)
	{
		if (MeleeDataAsset->bIsUsingCollisionComponent)
		{
			CollisionComponent->ClearMoveIgnoreActors();
			CollisionComponent->IgnoreActorWhenMoving(OwnerCharacter, true);

			CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AScWHandheld_Melee::OnCollisionComponentBeginOverlap);
		}
		else
		{
			
		}
	}
	SwingCounter = 0;

	BP_EndSwing();

	DefaultTracePatternIgnoredActorArray = { OwnerCharacter, this };

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
	if (InOtherActor == OwnerCharacter)
	{
		return;
	}
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
void AScWHandheld_Melee::BP_PreSwing_Implementation()
{
	++SwingCounter;
	BP_UpdateCurrentSwingVariantData();
}

void AScWHandheld_Melee::BP_BeginSwing_Implementation(float InSwingDamage, TSubclassOf<UDamageType> InSwingDamageTypeClass)
{
	LastSwingDamage = InSwingDamage;
	LastSwingDamageTypeClass = InSwingDamageTypeClass;
	LastSwingAffectedActorArray.Empty();

	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset);

	ensureReturn(OwnerCharacter);

	SwingPaticlesComponent = MeleeDataAsset->BP_InitializeSwingParticles(this);

	if (MeleeDataAsset->bIsUsingCollisionComponent)
	{
		ensureReturn(CollisionComponent);
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		BP_BeginTracePatterns();
	}
}

void AScWHandheld_Melee::BP_EndSwing_Implementation()
{
	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset);

	ensureReturn(OwnerCharacter);

	if (SwingPaticlesComponent)
	{
		SwingPaticlesComponent->DestroyComponent();
		SwingPaticlesComponent = nullptr;
	}
	if (MeleeDataAsset->bIsUsingCollisionComponent)
	{
		ensureReturn(CollisionComponent);
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AScWHandheld_Melee::BP_UpdateCurrentSwingVariantData_Implementation()
{
	const TArray<FScWMeleeSwingVariantData>* FinalVariantsArrayPtr = nullptr;

	ensureReturn(OwnerCharacter);
	UScWASC_Character* OwnerASC = OwnerCharacter->GetCharacterASC();
	ensureReturn(OwnerASC);

	const UScWComboData* OwnerRelevantCombo = OwnerASC->GetRelevantCombo();

	if (OwnerRelevantCombo && !OwnerRelevantCombo->OverrideSwingVariants.IsEmpty())
	{
		FinalVariantsArrayPtr = &OwnerRelevantCombo->OverrideSwingVariants;
	}
	else
	{
		UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
		ensureReturn(MeleeDataAsset);

		FinalVariantsArrayPtr = &MeleeDataAsset->SwingVariants;
	}
	ensureReturn(FinalVariantsArrayPtr);
	const TArray<FScWMeleeSwingVariantData>& FinalVariantsArray = *FinalVariantsArrayPtr;

	ensureReturn(!FinalVariantsArray.IsEmpty());
	CurrentSwingVariantData = FinalVariantsArray[SwingCounter % FinalVariantsArray.Num()];
}

bool AScWHandheld_Melee::BP_HandleSwingHit_Implementation(const FHitResult& InHitResult)
{
	ensureReturn(OwnerCharacter, false);

	AActor* HitActor = InHitResult.GetActor();
	ensureReturn(HitActor, false);

	if (LastSwingAffectedActorArray.Contains(HitActor))
	{
		return false;
	}
	else
	{
		LastSwingAffectedActorArray.Add(HitActor);

		FVector HitDirection = (InHitResult.TraceEnd - InHitResult.TraceStart).GetSafeNormal();
		UScWGameplayFunctionLibrary::ApplyPointDamage(this, HitActor, LastSwingDamage, HitDirection, InHitResult, OwnerCharacter->GetController(), LastSwingDamageTypeClass);
		return true;
	}
}
//~ End Swing

//~ Begin Patterns
FVector AScWHandheld_Melee::BP_GetPatternStartLocation_Implementation(const FScWMeleeSwingVariantData_TracePattern& InPatternData, int32 InPatternIndex) const
{
	ensureReturn(OwnerCharacter, GetActorLocation());
	return OwnerCharacter->GetPawnViewLocation() + OwnerCharacter->GetViewRotation().Vector() * InPatternData.TraceOffsetLocation;
}

float AScWHandheld_Melee::BP_GetNextPatternDelayTime_Implementation(int32 InNextPatternIndex) const
{
	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset, 0.0f);

	const auto& TracePatterns = CurrentSwingVariantData.TracePatterns;
	return (TracePatterns.Num() < 2) ? (0.0f) : (MeleeDataAsset->SwingVariantBaseDuration / (float)TracePatterns.Num());
}

void AScWHandheld_Melee::BP_BeginTracePatterns_Implementation()
{
	ensureReturn(!CurrentSwingVariantData.TracePatterns.IsEmpty());
	BP_HandleTracePattern(CurrentSwingVariantData.TracePatterns[0], 0);
}

void AScWHandheld_Melee::BP_HandleTracePattern_Implementation(const FScWMeleeSwingVariantData_TracePattern& InPatternData, int32 InPatternIndex)
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
	ActorsToIgnore.Append(DefaultTracePatternIgnoredActorArray);
	UKismetSystemLibrary::SphereTraceMulti(this, TraceStart, TraceEnd, InPatternData.TraceShapeRadius, TraceTypeQuery_Melee, false, ActorsToIgnore, TracePatternDebugType, TraceHitResults, true);
	
	if (TraceHitResults.IsEmpty())
	{
		return;
	}
	for (const FHitResult& SampleHitResult : TraceHitResults)
	{
		BP_HandleSwingHit(SampleHitResult);
	}
	int32 NextPatternIndex = InPatternIndex + 1;
	if (CurrentSwingVariantData.TracePatterns.IsValidIndex(NextPatternIndex))
	{
		float NextPatternDelayTime = BP_GetNextPatternDelayTime(NextPatternIndex);
		if (NextPatternDelayTime > 0.0f)
		{
			FTimerDelegate NextPatternMethodDelegate;
			NextPatternMethodDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED_TwoParams(AScWHandheld_Melee, BP_HandleTracePattern, const FScWMeleeSwingVariantData_TracePattern&, int32), InPatternData, NextPatternIndex);
			World->GetTimerManager().SetTimer(NextPatternDelayHandle, NextPatternMethodDelegate, NextPatternDelayTime, false);
		}
		else
		{
			BP_HandleTracePattern(InPatternData, NextPatternIndex);
		}
	}
}
//~ End Patterns
