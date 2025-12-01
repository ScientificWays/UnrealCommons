// Scientific Ways

#include "Gameplay/Characters/ScWCharacterCapsule.h"

#include "Gameplay/Characters/ScWCharacterData.h"

#include "Gameplay/ScWASC_Base.h"
#include "Gameplay/ScWGameplayTags.h"

UScWCharacterCapsule::UScWCharacterCapsule()
{
	TagCollisionProfileNameMap = {
		{ FScWGameplayTags::Collision_None, UCollisionProfile::NoCollision_ProfileName },
	};
}

//~ Begin Initialize
void UScWCharacterCapsule::InitFromASC(UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) // IScWASC_InitInterface
{
	ensureReturn(InInitASC);
	//ensureReturn(InOwnerActor);
	//ensureReturn(InAvatarActor);

	for (auto SampleTagProfile : TagCollisionProfileNameMap)
	{
		InInitASC->RegisterGameplayTagEvent(SampleTagProfile.Key, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnCollisionTagNumChanged);
	}
	//UpdateCollisionProfileFromTags();

	InInitASC->OnDamageApplied.AddDynamic(this, &ThisClass::HandleDamageApplied);
	InInitASC->OnDied.AddDynamic(this, &ThisClass::HandleDied);
}

void UScWCharacterCapsule::InitFromCharacterData(const UScWCharacterData* InInitCharacterData) // IScWCharacterData_InitInterface
{
	ensureReturn(InInitCharacterData);

	SetCapsuleSize(InInitCharacterData->CapsuleRadiusHeight.X, InInitCharacterData->CapsuleRadiusHeight.Y);
}

void UScWCharacterCapsule::BeginPlay() // UActorComponent
{
	DefaultCollisionProfileName = GetCollisionProfileName();
	UpdateCollisionProfileFromTags();

	Super::BeginPlay();
}
//~ End Initialize

//~ Begin Tags
void UScWCharacterCapsule::OnCollisionTagNumChanged(const FGameplayTag InCallbackTag, int32 InNewNum)
{
	UpdateCollisionProfileFromTags();
}

void UScWCharacterCapsule::UpdateCollisionProfileFromTags()
{
	UScWASC_Base* InitASC = GetInitASC();
	ensureReturn(InitASC);

	for (auto SampleTagProfile : TagCollisionProfileNameMap)
	{
		if (InitASC->HasMatchingGameplayTag(SampleTagProfile.Key))
		{
			SetCollisionProfileName(SampleTagProfile.Value);
			return;
		}
	}
	SetCollisionProfileName(DefaultCollisionProfileName);
}
//~ End Tags

//~ Begin Damage
void UScWCharacterCapsule::HandleDamageApplied(float InDamage, const FReceivedDamageData& InData)
{
	UScWASC_Base* InitASC = GetInitASC();
	ensureReturn(InitASC);

	
}

void UScWCharacterCapsule::HandleDied()
{
	UScWASC_Base* InitASC = GetInitASC();
	ensureReturn(InitASC);


}
//~ End Damage
