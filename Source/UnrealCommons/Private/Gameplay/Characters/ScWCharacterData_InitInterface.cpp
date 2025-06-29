// Scientific Ways

#include "Gameplay/Characters/ScWCharacterData_InitInterface.h"

#include "Gameplay/Characters/ScWCharacter.h"

void IScWCharacterData_InitInterface::HandleInit(AScWCharacter* InCharacter, const class UScWCharacterData* InInitCharacterData)
{
	TSet<UActorComponent*> TargetComponents = InCharacter->GetComponents();

	for (UActorComponent* TargetComponent : TargetComponents)
	{
		if (IScWCharacterData_InitInterface* TargetInterface = Cast<IScWCharacterData_InitInterface>(TargetComponent))
		{
			//ensure(TargetInterface->PrivateCharacterData == nullptr);
			TargetInterface->PrivateCharacterData = InInitCharacterData;
			TargetInterface->InitFromCharacterData(InInitCharacterData);
		}
	}
}
