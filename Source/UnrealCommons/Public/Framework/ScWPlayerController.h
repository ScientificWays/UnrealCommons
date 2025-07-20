// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_CommonDelegates.h"

#include "ScWPlayerController.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Player Controller"))
class UNREALCOMMONS_API AScWPlayerController : public APlayerController, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	AScWPlayerController();

//~ Begin Initialize
protected:
	virtual void PostInitializeComponents() override; // AActor
	virtual void BeginPlay() override; // AActor
	virtual void EndPlay(const EEndPlayReason::Type InReason) override; // AActor
//~ End Initialize
	
//~ Begin AbilitySystem
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface
//~ End AbilitySystem

//~ Begin Pawn
public:

	UPROPERTY(Category = "Pawn", BlueprintAssignable)
	FBaseASCEventSignature OnPawnHealthChangedDelegate;

	UPROPERTY(Category = "Pawn", BlueprintAssignable)
	FBaseASCEventSignature OnPawnMaxHealthChangedDelegate;

	UPROPERTY(Category = "Pawn", BlueprintAssignable)
	FBaseASCEventSignature OnPawnDiedDelegate;

protected:
	virtual void OnPossess(APawn* InPawn) override; // AController
	virtual void OnUnPossess() override; // AController
	virtual void PawnPendingDestroy(APawn* InPawn) override; // AController

	UFUNCTION()
	void BroadcastPawnHealthChanged(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue);

	UFUNCTION()
	void BroadcastPawnMaxHealthChanged(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue);

	UFUNCTION()
	void BroadcastPawnDied();

	FScriptDelegate OnPawnHealthChangedBind;
	FScriptDelegate OnPawnMaxHealthChangedBind;
	FScriptDelegate OnPawnDiedBind;
//~ End Pawn

//~ Begin Input
public:

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (DefaultToSelf = "InSource", KeyWords = "HasMouse, HasCursor"))
	bool HasShowMouseCursorSource(UObject* InSource) const { return ShowMouseCursorSourceSet.Contains(InSource); }

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (DefaultToSelf = "InSource", KeyWords = "AddMouse, AddCursor"))
	void AddShowMouseCursorSource(UObject* InSource) { ShowMouseCursorSourceSet.Add(InSource); }
	
	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (DefaultToSelf = "InSource", KeyWords = "RemoveMouse, RemoveCursor"))
	void RemoveShowMouseCursorSource(UObject* InSource) { ShowMouseCursorSourceSet.Remove(InSource); }

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (DefaultToSelf = "InSource", KeyWords = "HasBlock, HasIgnore"))
	bool HasMovementInputBlockSource(UObject* InSource) const { return MovementInputBlockSourceSet.Contains(InSource); }

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (DefaultToSelf = "InSource", KeyWords = "AddBlock, AddIgnore"))
	void AddMovementInputBlockSource(UObject* InSource) { MovementInputBlockSourceSet.Add(InSource); }

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (DefaultToSelf = "InSource", KeyWords = "RemoveBlock, RemoveIgnore"))
	void RemoveMovementInputBlockSource(UObject* InSource) { MovementInputBlockSourceSet.Remove(InSource); }

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (DefaultToSelf = "InSource", KeyWords = "HasBlock, HasIgnore"))
	bool HasLookInputBlockSource(UObject* InSource) const { return LookInputBlockSourceSet.Contains(InSource); }

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (DefaultToSelf = "InSource", KeyWords = "AddBlock, AddIgnore"))
	void AddLookInputBlockSource(UObject* InSource) { LookInputBlockSourceSet.Add(InSource); }

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (DefaultToSelf = "InSource", KeyWords = "RemoveBlock, RemoveIgnore"))
	void RemoveLookInputBlockSource(UObject* InSource) { LookInputBlockSourceSet.Remove(InSource); }

	UPROPERTY(Category = "Input", BlueprintReadWrite)
	float MouseInputScale;

protected:
	virtual void SetupInputComponent() override; // APlayerController
	virtual bool ShouldShowMouseCursor() const { return Super::ShouldShowMouseCursor() || !ShowMouseCursorSourceSet.IsEmpty(); } // APlayerController
	virtual bool IsMoveInputIgnored() const { return Super::IsMoveInputIgnored() || !MovementInputBlockSourceSet.IsEmpty(); } // AController
	virtual bool IsLookInputIgnored() const { return Super::IsLookInputIgnored() || !LookInputBlockSourceSet.IsEmpty(); } // AController
	void InputMouseLook(const FInputActionInstance& InActionInstance);

	UPROPERTY(Category = "Input", BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	int32 DefaultInputMappingContextPriority;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	FModifyContextOptions DefaultInputMappingContextOptions;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> MouseLookAction;

	UPROPERTY(Transient)
	TSet<TObjectPtr<UObject>> ShowMouseCursorSourceSet;

	UPROPERTY(Transient)
	TSet<TObjectPtr<UObject>> MovementInputBlockSourceSet;

	UPROPERTY(Transient)
	TSet<TObjectPtr<UObject>> LookInputBlockSourceSet;
//~ End Input
	
//~ Begin Team
public:
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; } // IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& InNewTeamID) override; // IGenericTeamAgentInterface
private:
	FGenericTeamId TeamId;
//~ End Team
	
//~ Begin Viewport
public:

	UFUNCTION(BlueprintCallable, Category = "Viewport")
	bool GetHitResultUnderScreenCenter(ETraceTypeQuery InTraceChannel, bool bInTraceComplex, FHitResult& OutHitResult) const;
//~ End Viewport
};
