// Scientific Ways

#include "Characters/DataAssets/ScWCharacterData.h"

#include "AI/ScWAIController.h"

#include "Characters/ScWCharacter.h"

#include "Framework/ScWGameState.h"

#include "Gameplay/ScWASC_Character.h"

UScWCharacterData::UScWCharacterData()
{
	DisplayName = FText::FromString(TEXT("Unnamed Character"));

	DefaultTeamName = TEXT("NoTeam");

	SkeletalMeshRelativeTransform = FTransform(FRotator(0.0f, -90.0, 0.0f), FVector(0.0f, 0.0f, -90.0f), FVector::OneVector);
	CapsuleRadiusHeight = FVector2D(34.0f, 90.0f);
}

void UScWCharacterData::K2_InitializeCharacterComponents_Implementation(AScWCharacter* InCharacter) const
{
	if (!InCharacter)
	{
		return;
	}
	if (UScWASC_Character* CharacterASC = InCharacter->GetCharacterASC())
	{
		CharacterASC->SpawnEffectClass = SpawnEffectClass;
		CharacterASC->SpawnAbilitiesGiveData = DefaultAbilitiesGiveData;
	}
	if (USkeletalMeshComponent* CharacterMesh = InCharacter->GetMesh())
	{
		CharacterMesh->SetSkeletalMeshAsset(SkeletalMesh);
		CharacterMesh->SetAnimInstanceClass(AnimInstanceClass);
		CharacterMesh->SetRelativeTransform(SkeletalMeshRelativeTransform);
	}
	if (UCapsuleComponent* CharacterCapsule = InCharacter->GetCapsuleComponent())
	{
		CharacterCapsule->SetCapsuleSize(CapsuleRadiusHeight.X, CapsuleRadiusHeight.Y);
	}
}

void UScWCharacterData::K2_InitializeCharacterController_Implementation(AScWCharacter* InCharacter) const
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
	if (AAIController* CharacterAIController = InCharacter->GetController<AAIController>())
	{
		CharacterAIController->RunBehaviorTree(DefaultBehaviorTree);
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
