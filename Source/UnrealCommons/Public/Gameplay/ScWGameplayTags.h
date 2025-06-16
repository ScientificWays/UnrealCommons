// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "NativeGameplayTags.h"

namespace FATAGameplayTags
{
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Sprint);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Cling);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Flashlight);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Weapon);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Weapon_Aim);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Weapon_Reload);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Ladder);

	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_AimActivate);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_EquipHolster);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_MeleeActivate);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_ReloadInputPress);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_SprintActivate);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_Stumbled);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_Captured);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_Incapped);

	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_OwnerDead);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_GameMode);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	// Initialization states for the GameFrameworkComponentManager, these are registered in order by IDGameInstance and some actors will skip right to GameplayReady
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned); // Actor/component has initially spawned and can be extended
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable); // All required data has been loaded/replicated and is ready for initialization
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized); // The available data has been initialized for this actor/component, but it is not ready for full gameplay
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady); // The actor/component is fully ready for active gameplay

	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Reset);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RequestReset);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Duration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Magnitude);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_MoveSpeed);

	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_Invulnerable);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_NoTarget);

	/*UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Aiming);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_BeingRevived);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Blocking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Captured);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Capturing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Crouching);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_EquipHolster);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_FlashlightOn);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Ghost);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Incapped);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Invisible);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Jumping);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Kamikaze_Dash);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Preview);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Reloading);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Riposting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Sprinting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Stumbled);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Swinging);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Ragdoll);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead_Started);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead_Finished);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_WalkingSlow);*/

	/*UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_BlockActivation_All);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_BlockActivation_Aim);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_BlockActivation_Weapon);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_BlockActivation_Sprint);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_BlockAttack_Bullet);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_BlockAttack_Melee);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_BlockAttack_Shove);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_BlockEffect_HealthRestore);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_IgnoreAttack_Bullet);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_IgnoreAttack_Melee);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_IgnoreAttack_Shove);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_HealthRestore);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Flash);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_ThirdPersonView);*/

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_None);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_Walking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_NavWalking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_Falling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_Swimming);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_Flying);

	/*UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_Custom_Ladder);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_Custom_Cling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_Custom_InVehicle);*/

	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Firearm_Shoot);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Firearm_Reload);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Firearm_Impact);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Melee_Swing);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Melee_Parry);
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Melee_Riposte);
}
