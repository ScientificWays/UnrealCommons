// Scientific Ways

#include "Framework/ScWPlayerController.h"

#include "Gameplay/ScWASC_Base.h"
#include "Gameplay/ScWGameplayFunctionLibrary.h"

AScWPlayerController::AScWPlayerController()
{
	MouseInputScale = 1.0f;

	TeamId = FGenericTeamId::NoTeam;
}

//~ Begin Initialize
void AScWPlayerController::PostInitializeComponents() // AActor
{
	Super::PostInitializeComponents();

	OnPawnHealthChangedBind.BindUFunction(this, TEXT("BroadcastPawnHealthChanged"));
	OnPawnMaxHealthChangedBind.BindUFunction(this, TEXT("BroadcastPawnMaxHealthChanged"));
	OnPawnDiedBind.BindUFunction(this, TEXT("BroadcastPawnDied"));
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

//~ Begin Pawn
void AScWPlayerController::OnPossess(APawn* InPawn) // AController
{
	Super::OnPossess(InPawn);

	if (UScWASC_Base* PawnASC = UScWASC_Base::TryGetBaseAtaASCFromActor(InPawn))
	{
		PawnASC->OnHealthChangedDelegate.Add(OnPawnHealthChangedBind);
		PawnASC->OnMaxHealthChangedDelegate.Add(OnPawnMaxHealthChangedBind);
		PawnASC->OnDiedDelegate.Add(OnPawnDiedBind);
	}
}

void AScWPlayerController::OnUnPossess() // AController
{
	Super::OnUnPossess();

	if (UScWASC_Base* PawnASC = UScWASC_Base::TryGetBaseAtaASCFromActor(GetPawn()))
	{
		PawnASC->OnHealthChangedDelegate.Remove(OnPawnHealthChangedBind);
		PawnASC->OnMaxHealthChangedDelegate.Remove(OnPawnMaxHealthChangedBind);
		PawnASC->OnDiedDelegate.Remove(OnPawnDiedBind);
	}
}

void AScWPlayerController::BroadcastPawnHealthChanged(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue)
{
	OnPawnHealthChangedDelegate.Broadcast(InAttribute, InPrevValue, InNewValue);
}

void AScWPlayerController::BroadcastPawnMaxHealthChanged(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue)
{
	OnPawnMaxHealthChangedDelegate.Broadcast(InAttribute, InPrevValue, InNewValue);
}

void AScWPlayerController::BroadcastPawnDied()
{
	OnPawnDiedDelegate.Broadcast();
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
	ensure(MouseLookAction);
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
