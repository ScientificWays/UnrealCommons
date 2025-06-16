// G.o.o.P Studios

#include "Characters/DataAssets/CharacterData.h"

#include "Characters/ATACharacter.h"

#include "GAS/ATAASC_Character.h"

#include "World/ATAGameState_Base.h"

UCharacterData::UCharacterData()
{
	DisplayName = FText::FromString(TEXT("Unnamed Character"));

	DefaultTeamName = TEXT("NoTeam");

	SkeletalMeshRelativeTransform = FTransform(FRotator(0.0f, -90.0, 0.0f), FVector(0.0f, 0.0f, -90.0f), FVector::OneVector);
	CapsuleRadiusHeight = FVector2D(34.0f, 90.0f);
}

void UCharacterData::K2_UpdateCharacterFromDataAsset_Implementation(AATACharacter* InCharacter) const
{
	if (!InCharacter)
	{
		return;
	}
	if (UATAASC_Character* CharacterASC = InCharacter->GetCharacterASC())
	{
		CharacterASC->SpawnEffectClass = SpawnEffectClass;
		CharacterASC->SpawnAbilities = DefaultAbilities;
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
	InCharacter->SetGenericTeamId(GetDefaultTeamId(InCharacter));

	if (AAIController* CharacterAIController = InCharacter->GetController<AAIController>())
	{
		CharacterAIController->RunBehaviorTree(DefaultBehaviorTree);
	}
}

//~ Begin Teams
FGenericTeamId UCharacterData::GetDefaultTeamId(const UObject* InWCO) const
{
	if (AATAGameState_Base* BaseGameState = AATAGameState_Base::TryGetAtaGameState(InWCO))
	{
		return BaseGameState->GetTeamId(DefaultTeamName);
	}
	return FGenericTeamId::NoTeam;
}
//~ End Teams
