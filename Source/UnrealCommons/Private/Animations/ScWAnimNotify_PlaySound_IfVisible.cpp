// Scientific Ways

#include "Animations/ScWAnimNotify_PlaySound_IfVisible.h"

#include "Gameplay/ScWGameplayFunctionLibrary.h"

UScWAnimNotify_PlaySound_IfVisible::UScWAnimNotify_PlaySound_IfVisible()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(196, 142, 255, 255);
	bPreviewIgnoreAttenuation = false;
#endif // WITH_EDITORONLY_DATA

//~ Begin Sound
	VolumeMultiplier = 1.0f;
	PitchMultiplier = 1.0f;
	bFollowOwner = true;
#if WITH_EDITORONLY_DATA
	bPreviewIgnoreAttenuation = false;
#endif
//~ End Sound
}

//~ Begin Initialize
void UScWAnimNotify_PlaySound_IfVisible::PostLoad() // UObject
{
	Super::PostLoad();

	
}
//~ End Initialize

//~ Begin Editor
#if WITH_EDITOR
void UScWAnimNotify_PlaySound_IfVisible::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) // UObject
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	
}

void UScWAnimNotify_PlaySound_IfVisible::ValidateAssociatedAssets() // UAnimNotify
{
	static const FName NAME_AssetCheck("AssetCheck");

	if (Sound != nullptr && !Sound->IsOneShot())
	{
		UObject* ContainingAsset = GetContainingAsset();

		FMessageLog AssetCheckLog(NAME_AssetCheck);

		const FText MessageLooping = FText::Format(
			NSLOCTEXT("AnimNotify", "Sound_ShouldNotLoop", "Sound {0} used in anim notify for asset {1} is set to looping, but the slot is a one-shot (it won't be played to avoid leaking an instance per notify)."),
			FText::AsCultureInvariant(Sound->GetPathName()),
			FText::AsCultureInvariant(ContainingAsset->GetPathName()));
		AssetCheckLog.Warning()
			->AddToken(FUObjectToken::Create(ContainingAsset))
			->AddToken(FTextToken::Create(MessageLooping));

		if (GIsEditor)
		{
			AssetCheckLog.Notify(MessageLooping, EMessageSeverity::Warning, /*bForce=*/ true);
		}
	}
}
#endif
//~ End Editor

//~ Begin Notify
FString UScWAnimNotify_PlaySound_IfVisible::GetNotifyName_Implementation() const // UAnimNotify
{
	if (Sound)
	{
		return Sound->GetName();
	}
	return Super::GetNotifyName_Implementation();
}

void UScWAnimNotify_PlaySound_IfVisible::Notify(USkeletalMeshComponent* InMeshComponent, UAnimSequenceBase* InAnimationSequenceBase, const FAnimNotifyEventReference& InEventReference) // UAnimNotify
{
	if (UScWGameplayFunctionLibrary::IsComponentRenderedFor(InMeshComponent, InMeshComponent->GetOwner()))
	{
		SpawnSound(InMeshComponent, InAnimationSequenceBase);
	}
}
//~ End Notify

//~ Begin Sound
void UScWAnimNotify_PlaySound_IfVisible::SpawnSound(USkeletalMeshComponent* InMeshComponent, UAnimSequenceBase* InAnimationSequenceBase)
{
	if (Sound && InMeshComponent)
	{
		if (!Sound->IsOneShot())
		{
			UE_LOG(LogTemp, Warning, TEXT("ScWAnimNotify_PlaySound_IfVisible notify: Animation %s tried to play a sound asset which is not a one-shot: '%s'. Spawning suppressed."), *GetNameSafe(InAnimationSequenceBase), *GetNameSafe(Sound));
			return;
		}
#if WITH_EDITORONLY_DATA
		UWorld* World = InMeshComponent->GetWorld();
		if (bPreviewIgnoreAttenuation && World && World->WorldType == EWorldType::EditorPreview)
		{
			UGameplayStatics::PlaySound2D(World, Sound, VolumeMultiplier, PitchMultiplier);
		}
		else
#endif
		{
			if (bFollowOwner)
			{
				UGameplayStatics::SpawnSoundAttached(Sound, InMeshComponent, AttachmentPoint, FVector(ForceInit), EAttachLocation::SnapToTarget, false, VolumeMultiplier, PitchMultiplier);
			}
			else
			{
				UGameplayStatics::PlaySoundAtLocation(InMeshComponent->GetWorld(), Sound, InMeshComponent->GetComponentLocation(), VolumeMultiplier, PitchMultiplier);
			}
		}
	}
}
//~ End Sound
