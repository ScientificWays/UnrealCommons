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

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/PlayerState.h"
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

#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Net/UnrealNetwork.h"

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

#define ensureContinue(InCondition) ensure(InCondition); if (!(InCondition)) continue;
#define ensureBreak(InCondition) ensure(InCondition); if (!(InCondition)) break;
#define ensureIf(InCondition) ensure(InCondition); if (InCondition)
#define ensureReturn(InCondition, ...) ensure(InCondition); if (!(InCondition)) return __VA_ARGS__;

#define TraceTypeQuery_Visibility				ETraceTypeQuery::TraceTypeQuery1
#define TraceTypeQuery_Camera					ETraceTypeQuery::TraceTypeQuery2
#define TraceTypeQuery_Melee					ETraceTypeQuery::TraceTypeQuery3

#define CollisionChannel_Visibility				ECollisionChannel::ECC_GameTraceChannel1
#define CollisionChannel_Camera					ECollisionChannel::ECC_GameTraceChannel2
#define CollisionChannel_Melee					ECollisionChannel::ECC_GameTraceChannel3

//#define ECC_Shoot								ECollisionChannel::ECC_GameTraceChannel1
//#define ECC_Interact							ECollisionChannel::ECC_GameTraceChannel2
//#define ECC_OnlyCharacterIgnore					ECollisionChannel::ECC_GameTraceChannel5
//#define ECC_OverlapAllShapes					ECollisionChannel::ECC_GameTraceChannel10

#define ObjectTypeQuery_WorldStatic				EObjectTypeQuery::ObjectTypeQuery1
#define ObjectTypeQuery_WorldDynamic			EObjectTypeQuery::ObjectTypeQuery2
#define ObjectTypeQuery_Pawn					EObjectTypeQuery::ObjectTypeQuery3
#define ObjectTypeQuery_PhysicsBody				EObjectTypeQuery::ObjectTypeQuery4
#define ObjectTypeQuery_Vehicle					EObjectTypeQuery::ObjectTypeQuery5
#define ObjectTypeQuery_Destructible			EObjectTypeQuery::ObjectTypeQuery6
//#define ObjectTypeQuery_PawnA					EObjectTypeQuery::ObjectTypeQuery7
//#define ObjectTypeQuery_PawnB					EObjectTypeQuery::ObjectTypeQuery8
//#define ObjectTypeQuery_Building				EObjectTypeQuery::ObjectTypeQuery9
//#define ObjectTypeQuery_Driver					EObjectTypeQuery::ObjectTypeQuery10
//#define ObjectTypeQuery_Volume					EObjectTypeQuery::ObjectTypeQuery11

class FUnrealCommonsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
