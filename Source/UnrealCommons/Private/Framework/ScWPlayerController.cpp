// Scientific Ways

#include "Framework/ScWPlayerController.h"

#include "Gameplay/ScWASC_Base.h"
#include "Gameplay/ScWGameplayFunctionLibrary.h"

AScWPlayerController::AScWPlayerController()
{
	bTryFindCalcCameraComponentOnPostInitializeComponents = true;

	MouseInputScale = 1.0f;

	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputMappingContext, DefaultInputMappingContext, "/UnrealCommons/Blueprints/Input/IMC_CommonPlayerController.IMC_CommonPlayerController");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, MouseLookAction, "/UnrealCommons/Blueprints/Input/IA_MouseLook.IA_MouseLook");

	TeamId = FGenericTeamId::NoTeam;
}

//~ Begin Initialize
void AScWPlayerController::PostInitializeComponents() // AActor
{
	Super::PostInitializeComponents();

	OnPawnHealthChangedBind.BindUFunction(this, TEXT("BroadcastPawnHealthChanged"));
	OnPawnMaxHealthChangedBind.BindUFunction(this, TEXT("BroadcastPawnMaxHealthChanged"));
	OnPawnDiedBind.BindUFunction(this, TEXT("BroadcastPawnDied"));

	if (bTryFindCalcCameraComponentOnPostInitializeComponents && (CalcCameraComponent == nullptr))
	{
		// Look for the first active camera component and assign that to CalcCameraComponent
		TInlineComponentArray<UCameraComponent*> CameraComponents;
		GetComponents(CameraComponents);

		for (UCameraComponent* SampleCameraComponent : CameraComponents)
		{
			if (SampleCameraComponent->IsActive())
			{
				CalcCameraComponent = SampleCameraComponent;
				break;
			}
		}
	}
}

void AScWPlayerController::BeginPlay() // AActor
{
	Super::BeginPlay();

	UScWGameplayFunctionLibrary::AddEnhancedInputMappingContextTo(this, DefaultInputMappingContext, DefaultInputMappingContextPriority, DefaultInputMappingContextOptions);
}

void AScWPlayerController::EndPlay(const EEndPlayReason::Type InReason) // AActor
{
	UScWGameplayFunctionLibrary::RemoveEnhancedInputMappingContextFrom(this, DefaultInputMappingContext, DefaultInputMappingContextOptions);

	Super::EndPlay(InReason);
}
//~ End Initialize

//~ Begin AbilitySystem
UAbilitySystemComponent* AScWPlayerController::GetAbilitySystemComponent() const // IAbilitySystemInterface
{
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPawn());
}
//~ End AbilitySystem

//~ Begin Camera
void AScWPlayerController::CalcCamera(float InDeltaSeconds, FMinimalViewInfo& InOutResult) // AActor
{
	if (CalcCameraComponent)
	{
		InOutResult.Location = CalcCameraComponent->GetComponentLocation();
		InOutResult.Rotation = CalcCameraComponent->GetComponentRotation();
	}
	else
	{
		Super::CalcCamera(InDeltaSeconds, InOutResult);
	}
}
//~ End Camera

//~ Begin Pawn
void AScWPlayerController::OnPossess(APawn* InPawn) // AController
{
	Super::OnPossess(InPawn);

	if (UScWASC_Base* PawnASC = UScWASC_Base::TryGetBaseScWASCFromActor(this))
	{
		PawnASC->OnHealthChanged.Add(OnPawnHealthChangedBind);
		PawnASC->OnMaxHealthChanged.Add(OnPawnMaxHealthChangedBind);
		PawnASC->OnDied.Add(OnPawnDiedBind);

		OnPawnHealthChangedDelegate.Broadcast(PawnASC);
		OnPawnMaxHealthChangedDelegate.Broadcast(PawnASC);
	}
}

void AScWPlayerController::OnUnPossess() // AController
{
	UScWASC_Base* PawnASC = UScWASC_Base::TryGetBaseScWASCFromActor(this);

	Super::OnUnPossess();

	if (PawnASC)
	{
		PawnASC->OnHealthChanged.Remove(OnPawnHealthChangedBind);
		PawnASC->OnMaxHealthChanged.Remove(OnPawnMaxHealthChangedBind);
		PawnASC->OnDied.Remove(OnPawnDiedBind);
	}
}

void AScWPlayerController::PawnPendingDestroy(APawn* InPawn) // AController
{
	//FVector ViewLocation;
	//FRotator ViewRotation;
	//GetPlayerViewPoint(ViewLocation, ViewRotation);
	
	Super::PawnPendingDestroy(InPawn);
}

void AScWPlayerController::BroadcastPawnHealthChanged(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue)
{
	if (UScWASC_Base* PawnASC = UScWASC_Base::TryGetBaseScWASCFromActor(this))
	{
		OnPawnHealthChangedDelegate.Broadcast(PawnASC);
	}
}

void AScWPlayerController::BroadcastPawnMaxHealthChanged(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue)
{
	if (UScWASC_Base* PawnASC = UScWASC_Base::TryGetBaseScWASCFromActor(this))
	{
		OnPawnMaxHealthChangedDelegate.Broadcast(PawnASC);
	}
}

void AScWPlayerController::BroadcastPawnDied()
{
	if (UScWASC_Base* PawnASC = UScWASC_Base::TryGetBaseScWASCFromActor(this))
	{
		OnPawnDiedDelegate.Broadcast(PawnASC);
	}
}
//~ End Pawn

//~ Begin Input
void AScWPlayerController::SetupInputComponent() // APlayerController
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	ensure(EnhancedInputComponent);
	if (!EnhancedInputComponent)
	{
		return;
	}
	//ensure(MouseLookAction);
	if (MouseLookAction)
	{
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AScWPlayerController::InputMouseLook);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Completed, this, &AScWPlayerController::InputMouseLook);
	}
}

void AScWPlayerController::InputMouseLook(const FInputActionInstance& InActionInstance)
{
	FVector2D InputValue = InActionInstance.GetValue().Get<FVector2D>();

	float ViewRoll = GetControlRotation().Roll;
	if (ViewRoll != 0.0f)
	{
		InputValue = InputValue.GetRotated(ViewRoll);
	}
	AddPitchInput(InputValue.Y * MouseInputScale);
	AddYawInput(InputValue.X * MouseInputScale);
}
//~ End Input

//~ Begin Team
void AScWPlayerController::SetGenericTeamId(const FGenericTeamId& InNewTeamId) // IGenericTeamAgentInterface
{
	TeamId = InNewTeamId;
}
//~ End Team

//~ Begin Viewport
bool AScWPlayerController::GetHitResultUnderScreenCenter(ETraceTypeQuery InTraceChannel, bool bInTraceComplex, FHitResult& OutHitResult) const
{
	FVector2D ViewportSize = FVector2D::ZeroVector;

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UGameViewportClient* ViewportClient = LocalPlayer->ViewportClient)
		{
			ViewportClient->GetViewportSize(ViewportSize);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return GetHitResultAtScreenPosition(ViewportSize * 0.5f, InTraceChannel, bInTraceComplex, OutHitResult);
}
//~ End Viewport
