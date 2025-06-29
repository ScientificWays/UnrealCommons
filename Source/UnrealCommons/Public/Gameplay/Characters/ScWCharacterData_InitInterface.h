// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWCharacterData_InitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UScWCharacterData_InitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALCOMMONS_API IScWCharacterData_InitInterface
{
	GENERATED_BODY()
	
	friend class UScWCharacterData;

//~ Begin Initialize
public:
	virtual void InitFromCharacterData(const class UScWCharacterData* InInitCharacterData) = 0; // IScWCharacterData_InitInterface
//~ End Initialize

//~ Begin DataAsset
public:
	const class UScWCharacterData* GetInitCharacterData() { return PrivateCharacterData; }
private:
	static void HandleInit(class AScWCharacter* InCharacter, const class UScWCharacterData* InInitCharacterData);
	TObjectPtr<const class UScWCharacterData> PrivateCharacterData;
//~ End DataAsset
};
