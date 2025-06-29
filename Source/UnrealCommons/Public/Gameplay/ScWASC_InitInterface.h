// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWASC_InitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UScWASC_InitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALCOMMONS_API IScWASC_InitInterface
{
	GENERATED_BODY()

	friend class UScWASC_Base;

//~ Begin Initialize
public:
	virtual void InitFromASC(class UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) = 0; // IScWASC_InitInterface
//~ End Initialize

//~ Begin Components | AbilitySystem
public:
	class UScWASC_Base* GetInitASC() const { return PrivateASC; }
private:
	static void HandleInit(class UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor);
	TObjectPtr<class UScWASC_Base> PrivateASC;
//~ End Components | AbilitySystem
};
