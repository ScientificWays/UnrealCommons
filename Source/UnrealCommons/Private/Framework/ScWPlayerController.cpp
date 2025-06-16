// Scientific Ways

#include "Framework/ScWPlayerController.h"

#include "Gameplay/ScWASC_Base.h"

AScWPlayerController::AScWPlayerController()
{
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

//~ Begin Team
void AScWPlayerController::SetGenericTeamId(const FGenericTeamId& InNewTeamId) // IGenericTeamAgentInterface
{
	TeamId = InNewTeamId;
}
//~ End Team

//~ Begin Input
//~ End Input
