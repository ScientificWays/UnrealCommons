// Scientific Ways

#include "Framework/ScWGameInstance.h"

UScWGameInstance::UScWGameInstance()
{
	
}

//~ Begin Statics
UScWGameInstance* UScWGameInstance::TryGetScWGameInstance(const UObject* InWCO)
{
	if (!InWCO)
	{
		UE_LOG(LogScWGameplay, Error, TEXT("UScWGameInstance::TryGetScWGameInstance() World context is not valid!"));
		return nullptr;
	}

	UWorld* World = InWCO->GetWorld();
	if (!World)
	{
		UE_LOG(LogScWGameplay, Error, TEXT("UScWGameInstance::TryGetScWGameInstance() World from context %s is not valid!"), *InWCO->GetName());
		return nullptr;
	}
	if (UScWGameInstance* OutGameInstance = World->GetGameInstance<UScWGameInstance>())
	{
		return OutGameInstance;
	}
	UE_LOG(LogScWGameplay, Error, TEXT("UScWGameInstance::TryGetScWGameInstance() GameInstance from %s is not of class UScWGameInstance!"), *World->GetName());
	return nullptr;
}
//~ End Statics
