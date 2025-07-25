// Scientific Ways

#include "Gameplay/ScWGameplayTags.h"

namespace FScWGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Flashlight, "Ability.Flashlight", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Movement_Jump, "Ability.Movement.Jump", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Movement_Sprint, "Ability.Movement.Sprint", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Movement_Crouch, "Ability.Movement.Crouch", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Movement_WalkSlow, "Ability.Movement.WalkSlow", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Handheld_Swing, "Ability.Handheld.Swing", "[Native] TODO: Melee weapon swing ability");
	/*UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Handheld, "Ability.Handheld", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Cling, "Ability.Cling", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Handheld_Aim, "Ability.Handheld.Aim", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Handheld_Reload, "Ability.Handheld.Reload", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Ladder, "Ability.Ladder", "[Native] TODO: Add description");*/

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_CancelBy_Stunned, "Ability.CancelBy.Stunned", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_CancelBy_JumpActivate, "Ability.CancelBy.JumpActivate", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_CancelBy_SprintActivate, "Ability.CancelBy.SprintActivate", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_CancelBy_CrouchActivate, "Ability.CancelBy.CrouchActivate", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_CancelBy_WalkSlowActivate, "Ability.CancelBy.WalkSlowActivate", "[Native] TODO: Add description");
	/*UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_CancelBy_AimActivate, "Ability.CancelBy.AimActivate", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_CancelBy_EquipHolster, "Ability.CancelBy.EquipHolster", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_CancelBy_MeleeActivate, "Ability.CancelBy.MeleeActivate", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_CancelBy_ReloadInputPress, "Ability.CancelBy.ReloadInputPress", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_CancelBy_Captured, "Ability.CancelBy.Captured", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_CancelBy_Incapped, "Ability.CancelBy.Incapped", "[Native] TODO: Add description");*/

	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_OwnerDead, "Ability.ActivateFail.IsDead", "[Native] Ability failed to activate because its owner is dead.");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cooldown, "Ability.ActivateFail.Cooldown", "[Native] Ability failed to activate because it is on cool down.");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cost, "Ability.ActivateFail.Cost", "[Native] Ability failed to activate because it did not pass the cost checks.");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_GameMode, "Ability.ActivateFail.GameMode", "[Native] Ability failed to activate because game mode is blocking it.");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsBlocked, "Ability.ActivateFail.TagsBlocked", "[Native] Ability failed to activate because tags are blocking it.");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsMissing, "Ability.ActivateFail.TagsMissing", "[Native] Ability failed to activate because tags are missing.");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Networking, "Ability.ActivateFail.Networking", "[Native] Ability failed to activate because it did not pass the network checks.");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_ActivationGroup, "Ability.ActivateFail.ActivationGroup", "[Native] Ability failed to activate because of its activation group.");

	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Behavior_SurvivesDeath, "Ability.Behavior.SurvivesDeath", "[Native] An ability with this type tag should not be canceled due to death.");

	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_Spawned, "InitState.Spawned", "[Native] 1: Actor/component has initially spawned and can be extended");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataAvailable, "InitState.DataAvailable", "[Native] 2: All required data has been loaded/replicated and is ready for initialization");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataInitialized, "InitState.DataInitialized", "[Native] 3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady, "InitState.GameplayReady", "[Native] 4: The actor/component is fully ready for active gameplay");

	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "[Native] Event that fires on death. This event only fires on the server.");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Reset, "GameplayEvent.Reset", "[Native] Event that fires once a player reset is executed.");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_RequestReset, "GameplayEvent.RequestReset", "[Native] Event to request a player's pawn to be instantly replaced with a new one at a valid spawn location.");

	// Set by caller
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Duration, "SetByCaller.Duration", "[Native] SetByCaller tag used by gameplay effects with custom duration.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Magnitude, "SetByCaller.Magnitude", "[Native] SetByCaller tag used by gameplay effects with custom magnitude.");

	// Cheats
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cheat_Invulnerable, "Cheat.Invulnerable", "[Native] Invulnerable cheat is active on the owner.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cheat_NoTarget, "Cheat.NoTarget", "[Native] NoTarget cheat is active on the owner.");

	// Movement states
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Jumping, "State.Jumping", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Sprinting, "State.Sprinting", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Crouching, "State.Crouching", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_WalkingSlow, "State.WalkingSlow", "[Native] TODO: Add description");

	// Capability states
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Stunned, "State.Stunned", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Ragdoll, "State.Ragdoll", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dead, "State.Dead", "[Native] Owner is dead but not yet destroyed (most likely to be destroyed in the next tick).");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dead_Started, "State.Dead.Started", "[Native] TODO: Add description");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dead_Finished, "State.Dead.Finished", "[Native] TODO: Add description");

	// Damage states
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_IgnoreAnyDamage, "State.IgnoreAnyDamage", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_BlockAnyDamage, "State.BlockAnyDamage", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_EvadeAnyDamage, "State.EvadeAnyDamage", "[Native] TODO: Add description");

	// Ability-driven states
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Swinging, "State.Swinging", "[Native] Melee swing state");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Aiming, "State.Aiming", "[Native] Weapon aim state");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_FlashlightOn, "State.FlashlightOn", "[Native] TODO: Add description");

	// Unsused states
	/*UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_BeingRevived, "State.BeingRevived", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Blocking, "State.Blocking", "[Native] Melee block state");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Captured, "State.Captured", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Capturing, "State.Capturing", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_EquipHolster, "State.EquipHolster", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Ghost, "State.Ghost", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Incapped, "State.Incapped", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Invisible, "State.Invisible", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Kamikaze_Dash, "State.Kamikaze.Dash", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Preview, "State.Preview", "[Native] Preview actor state");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Reloading, "State.Reloading", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Riposting, "State.Riposting", "[Native] TODO: Add description"); */

	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_BlockActivation_All, "Effect.BlockActivation.All", "[Native] TODO: Add description");
	/*UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_BlockAttack_Bullet, "Effect.BlockAttack.Bullet", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_BlockAttack_Melee, "Effect.BlockAttack.Melee", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_BlockAttack_Shove, "Effect.BlockAttack.Shove", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_BlockEffect_HealthRestore, "Effect.BlockEffect.HealthRestore", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_IgnoreAttack_Bullet, "Effect.IgnoreAttack.Bullet", "[Native] Bullet damage does not affect owner.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_IgnoreAttack_Melee, "Effect.IgnoreAttack.Melee", "[Native] Melee damage does not affect owner.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_IgnoreAttack_Shove, "Effect.IgnoreAttack.Shove", "[Native] Shove does not affect owner.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_HealthRestore, "Effect.HealthRestore", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_Flash, "Effect.Flash", "[Native] TODO: Add description");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_ThirdPersonView, "Effect.ThirdPersonView", "[Native] TODO: Add description"); */

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(MovementMode, "MovementMode", "[Native] Used to check for any movement mode tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(MovementMode_None, "MovementMode.None", "[Native] Character movement mode tag.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(MovementMode_Walking, "MovementMode.Walking", "[Native] Character movement mode tag.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(MovementMode_NavWalking, "MovementMode.NavWalking", "[Native] Character movement mode tag.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(MovementMode_Falling, "MovementMode.Falling", "[Native] Character movement mode tag.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(MovementMode_Swimming, "MovementMode.Swimming", "[Native] Character movement mode tag.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(MovementMode_Flying, "MovementMode.Flying", "[Native] Character movement mode tag.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Damage_Impact, "GameplayCue.Damage.Impact", "[Native] TODO: Add description");
}
