// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAnimNotify_NiagaraSystem_IfVisible.generated.h"

/**
 *	Same as UAnimNotify_PlayNiagaraEffect but checks if animated mesh rendered for its owner
 */
UCLASS(const, HideCategories = "Object", CollapseCategories, Config = "Game", meta = (DisplayName = "Play Niagara Particle Effect (If visible)"))
class UNREALCOMMONS_API UScWAnimNotify_NiagaraSystem_IfVisible : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	UScWAnimNotify_NiagaraSystem_IfVisible();

//~ Begin Initialize
protected:
	virtual void PostLoad() override; // UObject
//~ End Initialize

//~ Begin Editor
protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override; // UObject
	virtual void ValidateAssociatedAssets() override; // UAnimNotify
#endif
//~ End Editor
	
//~ Begin Notify
protected:
	virtual FString GetNotifyName_Implementation() const override; // UAnimNotify
	virtual void Notify(USkeletalMeshComponent* InMeshComponent, UAnimSequenceBase* InAnimationSequenceBase, const FAnimNotifyEventReference& InEventReference) override; // UAnimNotify
//~ End Notify

//~ Begin Niagara
public:
	
	UFUNCTION(Category = "Niagara", BlueprintCallable)
	UFXSystemComponent* GetSpawnedNiagaraSystem();

	UPROPERTY(Category = "Niagara", EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> NiagaraSystem;
	
	UPROPERTY(Category = "Niagara", EditAnywhere, AdvancedDisplay)
	bool bAttachParticles;

	UPROPERTY(Category = "Niagara", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bAttachParticles"))
	FName AttachmentPoint;

	UPROPERTY(Category = "Niagara", EditAnywhere, BlueprintReadWrite)
	FVector LocationOffset;

	UPROPERTY(Category = "Niagara", EditAnywhere, BlueprintReadWrite)
	FRotator RotationOffset;

	UPROPERTY(Category = "Niagara", EditAnywhere)
	FVector Scale;

	UPROPERTY(Category = "Niagara", EditAnywhere, AdvancedDisplay)
	bool bAbsoluteScale;

	UPROPERTY(Category = "Niagara", EditAnywhere, BlueprintReadWrite)
	FName FloatParameterToSet;

	UPROPERTY(Category = "Niagara", EditAnywhere, BlueprintReadWrite)
	float FloatParameterToSetValue;

protected:
	UFXSystemComponent* SpawnNiagaraSystem(USkeletalMeshComponent* InMeshComponent, UAnimSequenceBase* InAnimationSequenceBase);

	UPROPERTY()
	TObjectPtr<UFXSystemComponent> SpawnedNiagaraSystem;

	FQuat RotationOffsetQuat;
//~ End Niagara
};
