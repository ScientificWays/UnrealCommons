// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_Delegates.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE(FDefaultCallbackSignature);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDefaultEventSignature);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBoolSignature, bool, bInSuccess);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeChangedSignature, const FGameplayAttribute&, InAttribute, float, InPrevValue, float, InNewValue);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBaseASCEventSignature, const class UScWASC_Base*, InTargetASC);

//UDELEGATE()
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMovementInputEventSignature, float, InDeltaSeconds, float, InForwardValue, float, InRightValue);

//UDELEGATE()
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerStateSignature, class AIDPlayerState*, InPlayerState);

//UDELEGATE()
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerControllerSignature, class AIDPlayerController*, InPlayerController);

//UDELEGATE()
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterSignature, class AIDCharacter_Base*, InCharacter);

//UDELEGATE()
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityBaseSignature, class UIDGA_Base*, InAbility);

//UDELEGATE()
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTeamInterfaceSignature, TScriptInterface<class ITeamInterface>, InTeamInterface);

USTRUCT(BlueprintType)
struct FDelegateHandle_BlueprintWrapper
{
	GENERATED_BODY()

	FDelegateHandle Handle;

	operator FDelegateHandle() { return Handle; }
};
