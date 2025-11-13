// Scientific Ways

#include "Utils/ScWWorldObject.h"

UScWWorldObject::UScWWorldObject()
{
	
}

//~ Begin World
/*void UScWWorldObject::UpdateWorldContext(UObject* InWCO)
{
	CurrentWCO = InWCO;
}*/

UWorld* UScWWorldObject::GetWorld() const // UObject
{
	/*if (TStrongObjectPtr<UObject> SafeCurrentWCO = CurrentWCO.Pin())
	{
		if (!SafeCurrentWCO->HasAnyFlags(RF_BeginDestroyed) && !SafeCurrentWCO->IsUnreachable())
		{
			return SafeCurrentWCO->GetWorld();
		}
	}*/
	return GWorld;
}
//~ End World
