// Scientific Ways

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Delegates/IDelegateInstance.h"
#include "Delegates/DelegateCombinations.h"
#include "Engine/World.h"
#include "Engine/DamageEvents.h"
#include "Engine/LevelScriptActor.h"
#include "UObject/SparseDelegate.h"

#include "Net/UnrealNetwork.h"

#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AITypes.h"
#include "AIController.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "NavigationSystem.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AISense.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AIPerceptionComponent.h"

#include "AttributeSet.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayCueInterface.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

DECLARE_LOG_CATEGORY_EXTERN(LogScWGameplay, Log, All);

#define CONSTRUCTOR_TRY_LOAD_OBJECT(InClass, InProperty, InPath) \
	{ \
		static ConstructorHelpers::FObjectFinder<InClass> LoadedAsset(TEXT(InPath)); \
		if (LoadedAsset.Succeeded()) \
		{ \
			InProperty = LoadedAsset.Object; \
		} \
	}

#define CONSTRUCTOR_TRY_LOAD_CLASS(InClass, InProperty, InPath) \
	{ \
		static ConstructorHelpers::FClassFinder<InClass> LoadedAsset(TEXT(InPath)); \
		if (LoadedAsset.Succeeded()) \
		{ \
			InProperty = LoadedAsset.Class; \
		} \
	}

#define ensureReturn(InCondition, ...) ensure(InCondition); if (!InCondition) return __VA_ARGS__;

class FUnrealCommonsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
