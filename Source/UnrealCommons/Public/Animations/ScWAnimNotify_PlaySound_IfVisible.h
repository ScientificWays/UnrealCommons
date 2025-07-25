// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAnimNotify_PlaySound_IfVisible.generated.h"

/**
 *	Same as UAnimNotify_PlaySound but checks if animated mesh rendered for its owner
 */
UCLASS(const, HideCategories = "Object", CollapseCategories, Config = "Game", meta = (DisplayName = "Play Sound (If visible)"))
class UNREALCOMMONS_API UScWAnimNotify_PlaySound_IfVisible : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	UScWAnimNotify_PlaySound_IfVisible();

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

//~ Begin Sound
public:

	UPROPERTY(Category = "Sound", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TObjectPtr<USoundBase> Sound;

	UPROPERTY(Category = "Sound", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float VolumeMultiplier;

	UPROPERTY(Category = "Sound", EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float PitchMultiplier;

	UPROPERTY(Category = "Sound", EditAnywhere, BlueprintReadWrite)
	uint32 bFollowOwner:1;

	UPROPERTY(Category = "Sound", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bFollowOwner", ExposeOnSpawn = "true"))
	FName AttachmentPoint;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Sound", EditAnywhere, Config)
	uint32 bPreviewIgnoreAttenuation:1;
#endif
protected:
	void SpawnSound(USkeletalMeshComponent* InMeshComponent, UAnimSequenceBase* InSequenceAnimationBase);
//~ End Sound
};
