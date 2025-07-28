// Scientific Ways

#include "Gameplay/ScWGameplayFunctionLibrary.h"

#include "Framework/ScWGameState.h"

#include "Gameplay/ScWDamageType.h"
#include "Gameplay/Interact/ScWInteractComponent.h"

//~ Begin Health
TSubclassOf<UGameplayEffect> UScWGameplayFunctionLibrary::GetSetHealthGameplayEffectClass(const UObject* InWCO)
{
	const AScWGameState* BaseGameState = AScWGameState::TryGetScWGameState(InWCO);
	if (BaseGameState && BaseGameState->DefaultSetHealthGameplayEffectClass)
	{
		return BaseGameState->DefaultSetHealthGameplayEffectClass;
	}
	UE_LOG(LogScWGameplay, Error, TEXT("UScWGameplayFunctionLibrary::GetSetHealthGameplayEffectClass() GameState does not have valid DefaultSetHealthGameplayEffectClass!"));
	return nullptr;
}

TSubclassOf<UGameplayEffect> UScWGameplayFunctionLibrary::GetAddHealthGameplayEffectClass(const UObject* InWCO)
{
	const AScWGameState* BaseGameState = AScWGameState::TryGetScWGameState(InWCO);
	if (BaseGameState && BaseGameState->DefaultAddHealthGameplayEffectClass)
	{
		return BaseGameState->DefaultAddHealthGameplayEffectClass;
	}
	UE_LOG(LogScWGameplay, Error, TEXT("UScWGameplayFunctionLibrary::GetAddHealthGameplayEffectClass() GameState does not have valid DefaultAddHealthGameplayEffectClass!"));
	return nullptr;
}

TSubclassOf<UGameplayEffect> UScWGameplayFunctionLibrary::GetDeadStateGameplayEffectClass(const UObject* InWCO)
{
	const AScWGameState* BaseGameState = AScWGameState::TryGetScWGameState(InWCO);
	if (BaseGameState && BaseGameState->DefaultDeadStateGameplayEffectClass)
	{
		return BaseGameState->DefaultDeadStateGameplayEffectClass;
	}
	UE_LOG(LogScWGameplay, Error, TEXT("UScWGameplayFunctionLibrary::GetDeadStateGameplayEffectClass() GameState does not have valid DefaultDeadStateGameplayEffectClass!"));
	return nullptr;
}
//~ End Health

//~ Begin Damage
TSubclassOf<UGameplayEffect> UScWGameplayFunctionLibrary::GetApplyDamageGameplayEffectClassForType(const UObject* InWCO, const UDamageType* InDamageType)
{
	const UScWDamageType* ScWDamageType = Cast<UScWDamageType>(InDamageType);
	if (ScWDamageType && ScWDamageType->ApplyDamageGameplayEffectClass)
	{
		return ScWDamageType->ApplyDamageGameplayEffectClass;
	}
	const AScWGameState* BaseGameState = AScWGameState::TryGetScWGameState(InWCO);
	if (BaseGameState && BaseGameState->DefaultApplyDamageGameplayEffectClass)
	{
		return BaseGameState->DefaultApplyDamageGameplayEffectClass;
	}
	UE_LOG(LogScWGameplay, Error, TEXT("UScWGameplayFunctionLibrary::GetApplyDamageGameplayEffectClassForType() Neither GameState nor InDamageType (%s) have valid ApplyDamageGameplayEffectClass!"), *InDamageType->GetName());
	return nullptr;
}

float UScWGameplayFunctionLibrary::ApplyPointDamage(AActor* InSourceActor, AActor* InTargetActor, float InDamage, const FVector& InHitFromDirection, const FHitResult& InHitResult, AController* InInstigator, TSubclassOf<UDamageType> InDamageTypeClass)
{
	if (InTargetActor == nullptr)
	{
		return 0.0f;
	}
	if (InDamage <= 0.0f)
	{
		return 0.0f;
	}
	const TSubclassOf<UDamageType> ValidDamageTypeClass = InDamageTypeClass ? InDamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FPointDamageEvent PointDamageEvent = { InDamage, InHitResult, InHitFromDirection, ValidDamageTypeClass };

	return InTargetActor->TakeDamage(InDamage, PointDamageEvent, InInstigator, InSourceActor);
}

void UScWGameplayFunctionLibrary::MakeMinimalInfoDamageImpactHitResult(AActor* InSourceActor, UPrimitiveComponent* InSourceComponent, APawn* InInstigatorPawn, AActor* InDamagedActor, UPrimitiveComponent* InDamagedComponent, FHitResult& OutHitResult)
{
	ensure(InDamagedComponent);
	FVector SourceLocation = FVector::ZeroVector;

	if (InSourceComponent)
	{
		SourceLocation = InSourceComponent->GetComponentLocation();
	}
	else if (InSourceActor)
	{
		SourceLocation = InSourceActor->GetActorLocation();
	}
	else
	{
		ensure(false);
	}
	if (InInstigatorPawn)
	{
		SourceLocation = FMath::Lerp(SourceLocation, InInstigatorPawn->GetPawnViewLocation(), 0.5f);
	}
	FVector ImpactLocation = FVector::ZeroVector;

	if (InDamagedComponent)
	{
		ImpactLocation = InDamagedComponent->GetComponentLocation();
	}
	else if (InDamagedActor)
	{
		ImpactLocation = InDamagedActor->GetActorLocation();
	}
	else
	{
		ensure(false);
	}
	OutHitResult = FHitResult(InDamagedActor, InDamagedComponent, ImpactLocation, (SourceLocation - ImpactLocation).GetSafeNormal());
	OutHitResult.TraceStart = SourceLocation;
	OutHitResult.TraceEnd = ImpactLocation;
	OutHitResult.Time = 1.0f;
	OutHitResult.Distance = FVector::Distance(SourceLocation, ImpactLocation);
}
//~ End Damage

//~ Begin GameplayTags
FDelegateHandle_BlueprintWrapper UScWGameplayFunctionLibrary::RegisterGameplayTagEvent(UAbilitySystemComponent* InASC, FGameplayTagNumChangedEvent InEvent, FGameplayTag InTag, EGameplayTagEventType::Type InType)
{
	return { InASC->RegisterGameplayTagEvent(InTag, InType).AddUFunction(InEvent.GetUObject(), InEvent.GetFunctionName()) };
}

void UScWGameplayFunctionLibrary::UnRegisterGameplayTagEvent(UAbilitySystemComponent* InASC, FDelegateHandle_BlueprintWrapper InHandleWrapper, FGameplayTag InTag, EGameplayTagEventType::Type InType)
{
	InASC->RegisterGameplayTagEvent(InTag, InType).Remove(InHandleWrapper.Handle);
}
//~ End GameplayTags

//~ Begin Input
bool UScWGameplayFunctionLibrary::AddEnhancedInputMappingContextTo(APlayerController* InPlayerController, const UInputMappingContext* InMappingContext, int32 InPriority, const FModifyContextOptions& InOptions)
{
	if (InPlayerController && InMappingContext)
	{
		if (ULocalPlayer* LocalPlayer = InPlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(InMappingContext, InPriority, InOptions);
				return true;
			}
		}
	}
	return false;
}

bool UScWGameplayFunctionLibrary::RemoveEnhancedInputMappingContextFrom(APlayerController* InPlayerController, const UInputMappingContext* InMappingContext, const FModifyContextOptions& InOptions)
{
	if (InPlayerController && InMappingContext)
	{
		if (ULocalPlayer* LocalPlayer = InPlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->RemoveMappingContext(InMappingContext, InOptions);
				return true;
			}
		}
	}
	return false;
}

bool UScWGameplayFunctionLibrary::GetActionMappedKeyDisplayName(APlayerController* InPlayerController, const UInputAction* InInputAction, const bool bInLongDisplayName, FText& OutDisplayName)
{
	OutDisplayName = FText::GetEmpty();

	if (InPlayerController && InInputAction)
	{
		if (ULocalPlayer* LocalPlayer = InPlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				TArray<FKey> MappedKeys = Subsystem->QueryKeysMappedToAction(InInputAction);

				if (MappedKeys.Num() > 0)
				{
					OutDisplayName = MappedKeys[0].GetDisplayName(bInLongDisplayName);
					return true;
				}
			}
		}
	}
	return false;
}
//~ End Input

//~ Begin Visibility
bool UScWGameplayFunctionLibrary::IsComponentRenderedFor(UPrimitiveComponent* InComponent, AActor* ForActor)
{
	if (!InComponent->IsVisible())
	{
		return false;
	}
	else
	{
		if (APawn* OwnerPawn = Cast<APawn>(ForActor))
		{
			if (InComponent->bOwnerNoSee && OwnerPawn->IsLocallyViewed())
			{
				return false;
			}
			else if (InComponent->bOnlyOwnerSee && !OwnerPawn->IsLocallyViewed())
			{
				return false;
			}
		}
	}
	return true;
}
//~ End Visibility

//~ Begin Interact
TArray<AActor*> UScWGameplayFunctionLibrary::FilterActorsByCanInteract(const UObject* InWCO, UScWInteractComponent* InSourceInteractComponent, const TArray<AActor*>& InActorArray, const bool bReverseCondition, const bool bInReturnAllInsteadOfNothing, const bool bInReturnFirstValidActor)
{
	ensureReturn(InWCO, {});
	ensureReturn(InSourceInteractComponent, {});
	
	AActor* SourceOwnerActor = InSourceInteractComponent->GetOwner();
	TArray<AActor*> OutActorArray;

	for (AActor* SampleActor : InActorArray)
	{
		if (SampleActor == SourceOwnerActor)
		{
			continue;
		}
		if (UScWInteractComponent* SampleInteractComponent = SampleActor->FindComponentByClass<UScWInteractComponent>())
		{
			float InteractDuration = 0.0f;
			if (bReverseCondition != SampleInteractComponent->BP_CanReceiveInteractFrom(InSourceInteractComponent, InteractDuration))
			{
				OutActorArray.Add(SampleActor);

				if (bInReturnFirstValidActor)
				{
					return OutActorArray;
				}
			}
		}
	}
	if (bInReturnAllInsteadOfNothing && OutActorArray.IsEmpty())
	{
		return InActorArray;
	}
	return OutActorArray;
}

UScWInteractComponent* UScWGameplayFunctionLibrary::FindInteractTargetInLocation(const UObject* InWCO, const FVector& InLocation, const float InRadius, ETraceTypeQuery InTraceTypeQuery, const FScWTraceDebugData& InTraceDebugData, const TArray<AActor*>& InActorFilterArray)
{
	TArray<FHitResult> TraceHitResultArray;
	UKismetSystemLibrary::SphereTraceMulti(InWCO, InLocation, InLocation, InRadius, InTraceTypeQuery, false, InActorFilterArray, InTraceDebugData.DrawDebugType, TraceHitResultArray, false, InTraceDebugData.TraceColor, InTraceDebugData.TraceHitColor, InTraceDebugData.DrawTime);
	
	UScWInteractComponent* OutTarget = nullptr;
	float NearestDistanceSquared = FLT_MAX;

	for (const FHitResult& SampleHitResult : TraceHitResultArray)
	{
		AActor* SampleActor = SampleHitResult.GetActor();

		if (UScWInteractComponent* SampleComponent = SampleActor->FindComponentByClass<UScWInteractComponent>())
		{
			FVector SampleLocation = SampleActor->GetActorLocation();

			float SampleDistanceSquared = FVector::DistSquared(InLocation, SampleLocation);

			if (SampleDistanceSquared < NearestDistanceSquared)
			{
				FHitResult CheckHitResult;

				UKismetSystemLibrary::LineTraceSingle(InWCO, InLocation, SampleLocation, InTraceTypeQuery, false, {}, InTraceDebugData.DrawDebugType, CheckHitResult, false, InTraceDebugData.TraceColor, InTraceDebugData.TraceHitColor, InTraceDebugData.DrawTime);

				if (CheckHitResult.GetActor() == nullptr || CheckHitResult.GetActor() == SampleActor)
				{
					OutTarget = SampleComponent;

					NearestDistanceSquared = SampleDistanceSquared;
				}
			}
		}
	}
	return OutTarget;
}
//~ End Interact
