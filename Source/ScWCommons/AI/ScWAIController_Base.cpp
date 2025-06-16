// G.o.o.P Studios

#include "AI/ATAAIController_Base.h"

#include "AI/ATAPFC_Base.h"
#include "AI/ATAAIPC_Base.h"

AATAAIController_Base::AATAAIController_Base(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer.SetDefaultSubobjectClass<UATAPFC_Base>(TEXT("PathFollowingComponent")))
{
	BasePFC = Cast<UATAPFC_Base>(GetPathFollowingComponent());
	BasePFC->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);

	BaseAIPC = CreateDefaultSubobject<UATAAIPC_Base>("ATAAIPerceptionComponent");
	SetPerceptionComponent(*BaseAIPC);
}

//~ Begin Blackboard
bool AATAAIController_Base::InitializeBlackboard(UBlackboardComponent& InBlackboardComponent, UBlackboardData& InBlackboardAsset) // AAIController
{
	if (Super::InitializeBlackboard(InBlackboardComponent, InBlackboardAsset))
	{
		return true;
	}
	return false;
}
//~ End Blackboard

//~ Begin Team
void AATAAIController_Base::SetGenericTeamId(const FGenericTeamId& InNewTeamId) // IGenericTeamAgentInterface
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

//~ Begin Rotation
void AATAAIController_Base::ClearAllFocuses()
{
	for (FFocusKnowledge::FFocusItem& SamplePriority : FocusInformation.Priorities)
	{
		SamplePriority.Actor = nullptr;
		SamplePriority.Position = FAISystem::InvalidLocation;
	}
}

FVector AATAAIController_Base::GetFocalPointOnActor(const AActor* InActor) const // AAIController
{
	return InActor ? InActor->GetTargetLocation(const_cast<AATAAIController_Base*>(this)) : FAISystem::InvalidLocation;
}

void AATAAIController_Base::UpdateControlRotation(float InDeltaSeconds, bool bInUpdatePawn) // AAIController
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
