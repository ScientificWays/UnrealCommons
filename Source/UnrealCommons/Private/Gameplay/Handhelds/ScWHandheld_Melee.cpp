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

	PreSwingMontageSectionIndex = 0;
	SwingMontageSectionIndex = 1;
	EndSwingMontageSectionIndex = 2;
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

			if (!CollisionComponent->IsRegistered() || CollisionComponent->IsOwnerRunningUserConstructionScript())
			{
				CollisionComponent->SetAutoActivate(true);
			}
			CollisionComponent->SetActive(true);
		}
		else
		{
			if (!CollisionComponent->IsRegistered() || CollisionComponent->IsOwnerRunningUserConstructionScript())
			{
				CollisionComponent->SetAutoActivate(false);
			}
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

	BP_ResetSwingComponents();

	SwingHitIgnoredActors = { OwnerCharacter, this };
	TracePatternIgnoredActors = { OwnerCharacter, this };

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
		BP_HandleSwingHit(InOtherActor, InSweepHitResult);
	}
	else
	{
		FHitResult MinimalInfoHitResult;
		UScWGameplayFunctionLibrary::MakeMinimalInfoDamageImpactHitResult(this, InOverlappedComponent, OwnerCharacter, InOtherActor, InOtherComponent, MinimalInfoHitResult);
		BP_HandleSwingHit(InOtherActor, MinimalInfoHitResult);
	}
}
//~ End Components

//~ Begin Swing
float AScWHandheld_Melee::BP_GetSwingDamage_Implementation() const
{
	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset, 0.0f);

	float OutDamage = MeleeDataAsset->SwingBaseDamage;

	ensureReturn(OwnerCharacter, OutDamage);
	UScWASC_Character* OwnerASC = OwnerCharacter->GetCharacterASC();

	ensureReturn(OwnerASC, OutDamage);
	if (const UScWComboData* RelevantCombo = OwnerASC->GetRelevantCombo())
	{
		OutDamage = RelevantCombo->BP_ModifyHandheldDamage(MeleeDataAsset, OutDamage);
	}
	return OutDamage;
}

TSubclassOf<UScWDamageType> AScWHandheld_Melee::BP_GetSwingDamageTypeClass_Implementation() const
{
	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset, UScWDamageType::StaticClass());

	TSubclassOf<UScWDamageType> OutDamageTypeClass = MeleeDataAsset->SwingBaseDamageTypeClass;

	ensureReturn(OwnerCharacter, OutDamageTypeClass);
	UScWASC_Character* OwnerASC = OwnerCharacter->GetCharacterASC();

	ensureReturn(OwnerASC, OutDamageTypeClass);
	if (const UScWComboData* RelevantCombo = OwnerASC->GetRelevantCombo())
	{
		OutDamageTypeClass = RelevantCombo->BP_ModifyHandheldDamageTypeClass(MeleeDataAsset, OutDamageTypeClass);
	}
	return OutDamageTypeClass;
}

float AScWHandheld_Melee::BP_PreSwing_Implementation()
{
	CurrentSwingPhase = EScWSwingPhase::PreSwing;

	++SwingCounter;
	BP_UpdateCurrentSwingVariantData();

	const auto& CurrentSwingMontageData = CurrentSwingVariantData.MontageData;

	ensureReturn(CurrentSwingMontageData.GetRelevantTimingMontage(), -1.0f);
	ensureReturn(CurrentSwingMontageData.GetRelevantTimingMontage()->GetNumSections() >= 3, -1.0f);

	ensureReturn(OwnerCharacter, -1.0f);
	UScWAnimationsFunctionLibrary::PlayCharacterMontagesFromData(OwnerCharacter, CurrentSwingMontageData);

	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset, -1.0f);

	float OutPreSwingDelay = UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingMontageData, PreSwingMontageSectionIndex) * MeleeDataAsset->SwingVariantBaseDuration;
	OnPreSwingDelegate.Broadcast(OutPreSwingDelay);
	return OutPreSwingDelay;
}

float AScWHandheld_Melee::BP_BeginSwing_Implementation(float InSwingDamage, TSubclassOf<UDamageType> InSwingDamageTypeClass)
{
	CurrentSwingPhase = EScWSwingPhase::Swing;

	LastSwingDamage = InSwingDamage;
	LastSwingDamageTypeClass = InSwingDamageTypeClass;
	LastSwingAffectedActorArray.Empty();

	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset, -1.0f);

	SwingPaticlesComponent = MeleeDataAsset->BP_InitializeSwingParticles(this);

	if (MeleeDataAsset->bIsUsingCollisionComponent)
	{
		ensureReturn(CollisionComponent, -1.0f);
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		BP_BeginTracePatterns();
	}
	float OutSwingDuration = UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingVariantData.MontageData, SwingMontageSectionIndex) * MeleeDataAsset->SwingVariantBaseDuration;
	OnBeginSwingDelegate.Broadcast(OutSwingDuration);
	return OutSwingDuration;
}

float AScWHandheld_Melee::BP_EndSwing_Implementation(const bool bInWasCancelled)
{
	BP_ResetSwingComponents();

	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset, -1.0f);

	float OutEndSwingDelay = UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingVariantData.MontageData, EndSwingMontageSectionIndex) * MeleeDataAsset->SwingVariantBaseDuration;
	OnEndSwingDelegate.Broadcast(OutEndSwingDelay, bInWasCancelled);
	return OutEndSwingDelay;
}

void AScWHandheld_Melee::BP_ResetSwingComponents_Implementation()
{
	if (SwingPaticlesComponent)
	{
		SwingPaticlesComponent->DestroyComponent();
		SwingPaticlesComponent = nullptr;
	}
	UScWHandheldData_Melee* MeleeDataAsset = GetMeleeDataAsset();
	ensureReturn(MeleeDataAsset);

	if (MeleeDataAsset->bIsUsingCollisionComponent)
	{
		ensureReturn(CollisionComponent);
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	CurrentSwingPhase = EScWSwingPhase::None;
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

bool AScWHandheld_Melee::BP_HandleSwingHit_Implementation(AActor* InHitActor, const FHitResult& InHitResult)
{
	ensureReturn(CurrentSwingPhase == EScWSwingPhase::Swing, false);
	ensureReturn(InHitActor, false);

	if (SwingHitIgnoredActors.Contains(InHitActor) || LastSwingAffectedActorArray.Contains(InHitActor))
	{
		return false;
	}
	else
	{
		LastSwingAffectedActorArray.Add(InHitActor);

		FVector HitDirection = (InHitResult.TraceEnd - InHitResult.TraceStart).GetSafeNormal();

		ensureReturn(OwnerCharacter, false);
		UScWGameplayFunctionLibrary::ApplyPointDamage(this, InHitActor, LastSwingDamage, HitDirection, InHitResult, OwnerCharacter->GetController(), LastSwingDamageTypeClass);
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

	const float SwingDuration = UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingVariantData.MontageData, SwingMontageSectionIndex) * MeleeDataAsset->SwingVariantBaseDuration;
	const auto& TracePatterns = CurrentSwingVariantData.TracePatterns;
	return (TracePatterns.Num() < 2) ? (0.0f) : (SwingDuration / (float)TracePatterns.Num());
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
	UKismetSystemLibrary::SphereTraceMulti(this, TraceStart, TraceEnd, InPatternData.TraceShapeRadius, TraceTypeQuery_Melee, false, TracePatternIgnoredActors, TracePatternDebugType, TraceHitResults, true);
	
	/*if (TraceHitResults.IsEmpty())
	{
		return;
	}*/
	for (const FHitResult& SampleHitResult : TraceHitResults)
	{
		BP_HandleSwingHit(SampleHitResult.GetActor(), SampleHitResult);
	}
	int32 NextPatternIndex = InPatternIndex + 1;
	if (CurrentSwingVariantData.TracePatterns.IsValidIndex(NextPatternIndex))
	{
		float NextPatternDelayTime = BP_GetNextPatternDelayTime(NextPatternIndex);
		if (NextPatternDelayTime > 0.0f)
		{
			FTimerDelegate NextPatternMethodDelegate;
			NextPatternMethodDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED_TwoParams(ThisClass, BP_HandleTracePattern, const FScWMeleeSwingVariantData_TracePattern&, int32), CurrentSwingVariantData.TracePatterns[NextPatternIndex], NextPatternIndex);
			World->GetTimerManager().SetTimer(NextPatternDelayHandle, NextPatternMethodDelegate, NextPatternDelayTime, false);
		}
		else
		{
			BP_HandleTracePattern(InPatternData, NextPatternIndex);
		}
	}
}
//~ End Patterns
