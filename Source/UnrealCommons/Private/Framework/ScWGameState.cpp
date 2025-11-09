// Scientific Ways

#include "Framework/ScWGameState.h"

#include "Gameplay/Characters/ScWCharacter.h"

#include "Framework/ScWLevelScriptActor.h"

const FName AScWGameState::InvalidTeamName = TEXT("InvalidTeam");

AScWGameState::AScWGameState()
{
	bForceDefaultPlayerCharacterDataAsset = true;

	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, DefaultAddHealthGameplayEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonAddHealth");
	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, DefaultSetHealthGameplayEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonSetHealth");
	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, DefaultDeadStateGameplayEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonDeadState");
	//CONSTRUCTOR_TRY_LOAD_CLASS(UGameplayEffect, DefaultApplyDamageGameplayEffectClass, "/UnrealCommons/Blueprints/GameplayEffects/GE_CommonApplyDamage");
	
	TeamMap.Add(TEXT("NoTeam"), FGenericTeamId::NoTeam);
	TeamMap.Add(TEXT("Player"), FGenericTeamId(1u));
	TeamMap.Add(TEXT("Enemies"), FGenericTeamId(2u));

	SlowdownTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SlowdownTimeline"));
}

//~ Begin Statics
AScWGameState* AScWGameState::TryGetScWGameState(const UObject* InWCO)
{
	ensureReturn(InWCO, nullptr);
	UWorld* World = InWCO->GetWorld();

	ensureReturn(World, nullptr);
	AScWGameState* OutGameState = World->GetGameState<AScWGameState>();

	ensureReturn(OutGameState, nullptr);
	return OutGameState;
}
//~ End Statics

//~ Begin Initialize
void AScWGameState::BeginPlay() // AActor
{
	Super::BeginPlay();

	ensureReturn(SlowdownTimeline);

	SlowdownAlphaCurve = NewObject<UCurveFloat>();
	SlowdownAlphaCurve->FloatCurve.AddKey(0.0f, 0.0f);
	SlowdownAlphaCurve->FloatCurve.AddKey(1.0f, 1.0f);

	FOnTimelineFloat SlowdownUpdateFunction;
	SlowdownUpdateFunction.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AScWGameState, OnSlowdownTimelineUpdate));
	SlowdownTimeline->AddInterpFloat(SlowdownAlphaCurve, SlowdownUpdateFunction);

	FOnTimelineEvent SlowdownFinishedFunction;
	SlowdownFinishedFunction.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AScWGameState, OnSlowdownTimelineFinished));
	SlowdownTimeline->SetTimelineFinishedFunc(SlowdownFinishedFunction);

	SlowdownTimeline->SetLooping(false);
	SlowdownTimeline->SetPlayRate(0.5f);
}
//~ End Initialize

//~ Begin Data Assets
const UScWCharacterData* AScWGameState::BP_GetDataAssetForNewCharacter_Implementation(const AScWCharacter* InCharacter) const
{
	if (UWorld* World = GetWorld())
	{
		if (AScWLevelScriptActor* LevelScriptActor = Cast<AScWLevelScriptActor>(World->GetLevelScriptActor()))
		{
			if (const UScWCharacterData* OutCharacterData = LevelScriptActor->BP_GetDataAssetForNewCharacter(InCharacter))
			{
				return OutCharacterData;
			}
		}
	}
	if (InCharacter)
	{
		if (InCharacter->IsPlayerCharacter())
		{
			if (!InCharacter->GetDataAsset() || (bForceDefaultPlayerCharacterDataAsset && DefaultPlayerCharacterDataAsset))
			{
				return DefaultPlayerCharacterDataAsset;
			}
		}
		return InCharacter->GetDataAsset();
	}
	return nullptr;
}
//~ End Data Assets

//~ Begin Teams
bool AScWGameState::HasTeamName(const FName& InTeamName) const
{
	return TeamMap.Contains(InTeamName);
}

FGenericTeamId AScWGameState::GetTeamId(const FName& InTeamName) const
{
	const FGenericTeamId* OutTeamIdPtr = TeamMap.Find(InTeamName);
	ensureReturn(OutTeamIdPtr, FGenericTeamId::NoTeam);
	return *OutTeamIdPtr;
}

const FName& AScWGameState::GetTeamName(FGenericTeamId InGenericTeamId) const
{
	const FName* OutName = TeamMap.FindKey(InGenericTeamId.GetId());
	ensureReturn(OutName, InvalidTeamName);
	return *OutName;
}
//~ End Teams

//~ Begin Pause
void AScWGameState::AddPauseSourceObject(UObject* InSourceObject)
{
	PauseSourceObjectsSet.Add(InSourceObject);
	UpdatePauseState();
}

void AScWGameState::RemovePauseSourceObject(UObject* InSourceObject)
{
	PauseSourceObjectsSet.Remove(InSourceObject);
	UpdatePauseState();
}

void AScWGameState::SetSlowdownRate(float InRate)
{
	ensureReturn(SlowdownTimeline);
	SlowdownTimeline->SetPlayRate(InRate);
}

void AScWGameState::UpdatePauseState()
{
	ensureReturn(SlowdownTimeline);

	if (PauseSourceObjectsSet.IsEmpty())
	{
		SlowdownTargetTimeDilation = 1.0f;
		SlowdownTimeline->Reverse();
	}
	else
	{
		SlowdownTargetTimeDilation = 0.0f;
		SlowdownTimeline->Play();
	}
}

void AScWGameState::OnSlowdownTimelineUpdate(float InValue)
{
	UWorld* World = GetWorld();
	ensureReturn(World);

	AWorldSettings* WorldSettings = World->GetWorldSettings();
	ensureReturn(WorldSettings);

	WorldSettings->SetTimeDilation(FMath::Lerp(WorldSettings->TimeDilation, SlowdownTargetTimeDilation, InValue));
}

void AScWGameState::OnSlowdownTimelineFinished()
{
	UGameInstance* GameInstance = GetGameInstance();
	ensureReturn(GameInstance);

	APlayerController* FirstPlayerController = GameInstance->GetFirstLocalPlayerController();
	ensureReturn(FirstPlayerController);

	FirstPlayerController->SetPause(SlowdownTargetTimeDilation < KINDA_SMALL_NUMBER);
}
//~ End Pause
