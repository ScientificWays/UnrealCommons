// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_CommonDelegates.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE(FDefaultCallbackSignature);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDefaultEventSignature);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBoolSignature, bool, bInValue);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSuccessSignature, bool, bInSuccess);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWasCancelledSignature, bool, bInWasCancelled);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeChangedSignature, const FGameplayAttribute&, InAttribute, float, InPrevValue, float, InNewValue);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBaseASCEventSignature, const class UScWASC_Base*, InTargetASC);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScWCharacterSignature, class AScWCharacter*, InCharacter);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTeamNameSignature, const FName&, InTeamName);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScWWidgetClassSignature, TSubclassOf<UScWUserWidget>, InWidgetClass);

USTRUCT(BlueprintType)
struct FDelegateHandle_BlueprintWrapper
{
	GENERATED_BODY()

	FDelegateHandle Handle;

	operator FDelegateHandle() { return Handle; }
};
