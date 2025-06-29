// Scientific Ways

#include "AI/ScWAIController.h"

#include "AI/ScWPFC_Base.h"
#include "AI/ScWAIPC_Base.h"
#include "AI/ScWAIFunctionLibrary.h"

AScWAIController::AScWAIController(const FObjectInitializer& InObjectInitializer)
//	: Super(InObjectInitializer.SetDefaultSubobjectClass<UScWPFC_Base>(TEXT("PathFollowingComponent")))
	: Super(InObjectInitializer.DoNotCreateDefaultSubobject(TEXT("PathFollowingComponent")))
{
	//BasePFC = Cast<UScWPFC_Base>(GetPathFollowingComponent());
	BasePFC = CreateDefaultSubobject<UScWPFC_Base>(TEXT("BasePFC"));
	BasePFC->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
	SetPathFollowingComponent(BasePFC);

	BaseAIPC = CreateDefaultSubobject<UScWAIPC_Base>("BaseAIPC");
	SetPerceptionComponent(*BaseAIPC);
}

//~ Begin Blackboard
bool AScWAIController::InitializeBlackboard(UBlackboardComponent& InBlackboardComponent, UBlackboardData& InBlackboardAsset) // AAIController
{
	if (Super::InitializeBlackboard(InBlackboardComponent, InBlackboardAsset))
	{
		return true;
	}
	return false;
}
//~ End Blackboard

//~ Begin Team
void AScWAIController::SetGenericTeamId(const FGenericTeamId& InNewTeamId) // IGenericTeamAgentInterface
{
	Super::SetGenericTeamId(InNewTeamId);

	if (BaseAIPC)
	{
		if (UAIPerceptionSystem* AIPerceptionSys = UAIPerceptionSystem::GetCurrent(GetWorld()))
		{
			AIPerceptionSys->UpdateListener(*BaseAIPC);
		}
	}
}
//~ End Team

//~ Begin Navigation
AScWAIPatrolPoint* AScWAIController::BP_GetRelevantPatrolPoint_Implementation(const FVector& InReferenceLocation, int32 InDesiredPathPointOffset) const
{
	int32 NearestPointIndex = UScWAIFunctionLibrary::GetNearestPatrolPointIndex(InReferenceLocation, CurrentPatrolPoints, true);
	if (CurrentPatrolPoints.IsValidIndex(NearestPointIndex))
	{
		return CurrentPatrolPoints[(NearestPointIndex + InDesiredPathPointOffset) % CurrentPatrolPoints.Num()];
	}
	return nullptr;
}
//~ End Navigation

//~ Begin AbilitySystem
UAbilitySystemComponent* AScWAIController::GetAbilitySystemComponent() const // IAbilitySystemInterface
{
	if (IAbilitySystemInterface* PawnAbilitySystemInterface = GetPawn<IAbilitySystemInterface>())
	{
		return PawnAbilitySystemInterface->GetAbilitySystemComponent();
	}
	return nullptr;
}
//~ End AbilitySystem

//~ Begin Rotation
void AScWAIController::ClearAllFocuses()
{
	for (FFocusKnowledge::FFocusItem& SamplePriority : FocusInformation.Priorities)
	{
		SamplePriority.Actor = nullptr;
		SamplePriority.Position = FAISystem::InvalidLocation;
	}
}

FVector AScWAIController::GetFocalPointOnActor(const AActor* InActor) const // AAIController
{
	return InActor ? InActor->GetTargetLocation(const_cast<AScWAIController*>(this)) : FAISystem::InvalidLocation;
}

void AScWAIController::UpdateControlRotation(float InDeltaSeconds, bool bInUpdatePawn) // AAIController
{
	if (APawn* ControlledPawn = GetPawn())
	{
		FRotator NewControlRotation = GetControlRotation();

		if (ControlRotationBlockSet.IsEmpty())
		{
			const FVector FocalPoint = GetFocalPoint();

			if (FAISystem::IsValidLocation(FocalPoint))
			{
				FQuat CurrentQuat = GetControlRotation().Quaternion();
				FQuat TargetQuat = (FocalPoint - ControlledPawn->GetPawnViewLocation()).ToOrientationQuat();

				if (!CurrentQuat.Equals(TargetQuat, 1e-3f))
				{
					NewControlRotation = FQuat::Slerp(CurrentQuat, TargetQuat, FMath::Min(FocusInterpSpeed * InDeltaSeconds, 1.0f)).Rotator();
					SetControlRotation(NewControlRotation);
				}
			}
			else if (bSetControlRotationFromPawnOrientation)
			{
				NewControlRotation = ControlledPawn->GetActorRotation();
				SetControlRotation(NewControlRotation);
			}
		}
		if (bInUpdatePawn)
		{
			const FRotator CurrentPawnRotation = ControlledPawn->GetActorRotation();

			if (!CurrentPawnRotation.Equals(NewControlRotation, 1e-3f))
			{
				ControlledPawn->FaceRotation(NewControlRotation, InDeltaSeconds);
			}
		}
	}
}
//~ End Rotation
