// Scientific Ways

#include "Gameplay/Characters/ScWCharacterData.h"

#include "AI/ScWAIPC_Base.h"
#include "AI/ScWAIController.h"

#include "Gameplay/Characters/ScWCharacterData_InitInterface.h"

#include "Framework/ScWGameState.h"

#include "Perception/AISenseConfig_Sight.h"

UScWCharacterData::UScWCharacterData()
{
	DisplayName = FText::FromString(TEXT("Unnamed Character"));

	DefaultTeamName = TEXT("NoTeam");

	bRagdollOnDeath = true;

	SkeletalMeshRelativeTransform = FTransform(FRotator(0.0f, -90.0, 0.0f), FVector(0.0f, 0.0f, -90.0f), FVector::OneVector);
	CapsuleRadiusHeight = FVector2D(34.0f, 90.0f);
}

void UScWCharacterData::BP_InitializeCharacterComponents_Implementation(AScWCharacter* InCharacter) const
{
	ensureReturn(InCharacter);
	IScWCharacterData_InitInterface::HandleInit(InCharacter, this);
}

void UScWCharacterData::BP_InitializeCharacterController_Implementation(AScWCharacter* InCharacter) const
{
	if (!InCharacter)
	{
		return;
	}
	InCharacter->SetGenericTeamId(GetDefaultTeamId(InCharacter));

	if (AIControllerClass)
	{
		InCharacter->AIControllerClass = AIControllerClass;
	}
	if (AScWAIController* CharacterAIController = InCharacter->GetController<AScWAIController>())
	{
		ensureReturn(DefaultBehaviorTree);
		CharacterAIController->RunBehaviorTree(DefaultBehaviorTree);

		UAISenseConfig_Sight* SightConfig = CharacterAIController->GetBaseAIPC()->GetSenseConfig<UAISenseConfig_Sight>();
		ensureReturn(SightConfig);
		SightConfig->SightRadius = SightRadius;
		SightConfig->LoseSightRadius = SightRadius + 100.0f;
	}
}

//~ Begin Teams
FGenericTeamId UScWCharacterData::GetDefaultTeamId(const UObject* InWCO) const
{
	if (AScWGameState* BaseGameState = AScWGameState::TryGetScWGameState(InWCO))
	{
		return BaseGameState->GetTeamId(DefaultTeamName);
	}
	return FGenericTeamId::NoTeam;
}
//~ End Teams
