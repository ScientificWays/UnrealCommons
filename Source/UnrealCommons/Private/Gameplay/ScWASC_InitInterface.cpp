// Scientific Ways

#include "Gameplay/ScWASC_InitInterface.h"

void IScWASC_InitInterface::HandleInit(class UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor)
{
	TSet<UActorComponent*> TargetComponents;
	if (InOwnerActor)
	{
		TargetComponents.Append(InOwnerActor->GetComponents());
	}
	if (InAvatarActor)
	{
		TargetComponents.Append(InAvatarActor->GetComponents());
	}
	for (UActorComponent* TargetComponent : TargetComponents)
	{
		if (IScWASC_InitInterface* TargetInterface = Cast<IScWASC_InitInterface>(TargetComponent))
		{
			ensure(TargetInterface->PrivateASC == nullptr);
			TargetInterface->PrivateASC = InInitASC;
			TargetInterface->InitFromASC(InInitASC, InOwnerActor, InAvatarActor);
		}
	}
}
