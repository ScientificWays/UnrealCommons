// Scientific Ways

#include "Gameplay/Characters/ScWCharacterMesh_Base.h"

#include "Gameplay/Characters/ScWCharacter.h"
#include "Gameplay/Characters/ScWCharacterData.h"

#include "Gameplay/ScWASC_Base.h"
#include "Gameplay/Handhelds/ScWHandheld.h"
#include "Gameplay/Handhelds/ScWHandheldData.h"

UScWCharacterMesh_Base::UScWCharacterMesh_Base()
{
	
}

//~ Begin Initialize
void UScWCharacterMesh_Base::OnRegister() // UActorComponent
{
	Super::OnRegister();

	OwnerCharacter = GetOwner<AScWCharacter>();
	ensureReturn(OwnerCharacter);
}

void UScWCharacterMesh_Base::BeginPlay() // UActorComponent
{
	Super::BeginPlay();

	ensureReturn(OwnerCharacter);
	OwnerCharacter->OnHandheldChanged.AddDynamic(this, &ThisClass::OnOwnerHandheldChanged);
	UpdateFromHandheld(OwnerCharacter->GetHandheld());

	OwnerCharacter->OnIsInFirstPersonViewChangedDelegate.AddDynamic(this, &ThisClass::OnOwnerIsInFirstPersonViewChanged);
	UpdateFromIsInFirstPersonView(OwnerCharacter->IsInFirstPersonView());
}

void UScWCharacterMesh_Base::InitFromCharacterData(const UScWCharacterData* InInitCharacterData) // IScWCharacterData_InitInterface
{
	ensureReturn(InInitCharacterData);

	
}

void UScWCharacterMesh_Base::InitFromASC(UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) // IScWASC_InitInterface
{
	ensureReturn(InInitASC);
	//ensureReturn(InOwnerActor);
	//ensureReturn(InAvatarActor);

	
}
//~ End Initialize

//~ Begin Updates
void UScWCharacterMesh_Base::OnOwnerIsInFirstPersonViewChanged(const bool bInIsInFirstPersonView)
{
	UpdateFromIsInFirstPersonView(bInIsInFirstPersonView);
}

void UScWCharacterMesh_Base::OnOwnerHandheldChanged(AScWHandheld* InPrevHandheld, AScWHandheld* InNewHandheld)
{
	ensure((InNewHandheld == nullptr) || (InNewHandheld->GetDataAsset() != nullptr));
	UpdateFromHandheld(InNewHandheld);
}

void UScWCharacterMesh_Base::UpdateFromIsInFirstPersonView(bool bInIsInFirstPersonView)
{

}

void UScWCharacterMesh_Base::UpdateFromHandheld(AScWHandheld* InHandheld)
{
	
}
//~ End Updates
