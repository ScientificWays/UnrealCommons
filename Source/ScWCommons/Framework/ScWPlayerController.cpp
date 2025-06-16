// G.o.o.P Studios

#include "Player/ATAPlayerController.h"

#include "GAS/ATAASC_Base.h"

AATAPlayerController::AATAPlayerController()
{
	TeamId = FGenericTeamId::NoTeam;
}

//~ Begin Initialize
void AATAPlayerController::PostInitializeComponents() // AActor
{
	Super::PostInitializeComponents();

	OnPawnHealthChangedBind.BindUFunction(this, TEXT("BroadcastPawnHealthChanged"));
	OnPawnMaxHealthChangedBind.BindUFunction(this, TEXT("BroadcastPawnMaxHealthChanged"));
	OnPawnDiedBind.BindUFunction(this, TEXT("BroadcastPawnDied"));
}

void AATAPlayerController::BeginPlay() // AActor
{
	Super::BeginPlay();

	
}
//~ End Initialize

//~ Begin Pawn
void AATAPlayerController::OnPossess(APawn* InPawn) // AController
{
	Super::OnPossess(InPawn);

	if (UATAASC_Base* PawnASC = UATAASC_Base::TryGetBaseAtaASCFromActor(InPawn))
	{
		PawnASC->OnHealthChangedDelegate.Add(OnPawnHealthChangedBind);
		PawnASC->OnMaxHealthChangedDelegate.Add(OnPawnMaxHealthChangedBind);
		PawnASC->OnDiedDelegate.Add(OnPawnDiedBind);
	}
}

void AATAPlayerController::OnUnPossess() // AController
{
	Super::OnUnPossess();

	if (UATAASC_Base* PawnASC = UATAASC_Base::TryGetBaseAtaASCFromActor(GetPawn()))
	{
		PawnASC->OnHealthChangedDelegate.Remove(OnPawnHealthChangedBind);
		PawnASC->OnMaxHealthChangedDelegate.Remove(OnPawnMaxHealthChangedBind);
		PawnASC->OnDiedDelegate.Remove(OnPawnDiedBind);
	}
}

void AATAPlayerController::BroadcastPawnHealthChanged(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue)
{
	OnPawnHealthChangedDelegate.Broadcast(InAttribute, InPrevValue, InNewValue);
}

void AATAPlayerController::BroadcastPawnMaxHealthChanged(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue)
{
	OnPawnMaxHealthChangedDelegate.Broadcast(InAttribute, InPrevValue, InNewValue);
}

void AATAPlayerController::BroadcastPawnDied()
{
	OnPawnDiedDelegate.Broadcast();
}
//~ End Pawn

//~ Begin Team
void AATAPlayerController::SetGenericTeamId(const FGenericTeamId& InNewTeamId) // IGenericTeamAgentInterface
{
	TeamId = InNewTeamId;
}
//~ End Team

//~ Begin Input
//~ End Input
