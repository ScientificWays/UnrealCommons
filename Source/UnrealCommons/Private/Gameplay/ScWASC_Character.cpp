// Scientific Ways

#include "Gameplay/ScWASC_Character.h"

#include "Gameplay/Attributes/ScWAS_Character.h"

UScWASC_Character::UScWASC_Character()
{
	AttributeSetClass = UScWAS_Character::StaticClass();

	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, SpawnEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonCharacterInit");
}

//~ Begin Statics
UScWASC_Character* UScWASC_Character::TryGetCharacterScWASCFromActor(const AActor* InActor, bool bInTryFindComponentIfNoInterface)
{
	return Cast<UScWASC_Character>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InActor, bInTryFindComponentIfNoInterface));
}
//~ End Statics
