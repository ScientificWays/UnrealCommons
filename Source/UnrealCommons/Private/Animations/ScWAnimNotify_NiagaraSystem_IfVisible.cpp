// Scientific Ways

#include "Animations/ScWAnimNotify_NiagaraSystem_IfVisible.h"

#include "Gameplay/ScWGameplayFunctionLibrary.h"

UScWAnimNotify_NiagaraSystem_IfVisible::UScWAnimNotify_NiagaraSystem_IfVisible()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(196, 142, 255, 255);
#endif // WITH_EDITORONLY_DATA
	
//~ Begin Niagara
	Scale = FVector::OneVector;
	bAbsoluteScale = false;
	bAttachParticles = true;
//~ End Niagara
}

//~ Begin Initialize
void UScWAnimNotify_NiagaraSystem_IfVisible::PostLoad() // UObject
{
	Super::PostLoad();

	RotationOffsetQuat = FQuat(RotationOffset);
}
//~ End Initialize

//~ Begin Editor
#if WITH_EDITOR
void UScWAnimNotify_NiagaraSystem_IfVisible::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) // UObject
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, RotationOffset))
	{
		RotationOffsetQuat = FQuat(RotationOffset);
	}
}

void UScWAnimNotify_NiagaraSystem_IfVisible::ValidateAssociatedAssets() // UAnimNotify
{
	static const FName NAME_AssetCheck("AssetCheck");

	if ((NiagaraSystem != nullptr) && (NiagaraSystem->IsLooping()))
	{
		UObject* ContainingAsset = GetContainingAsset();

		FMessageLog AssetCheckLog(NAME_AssetCheck);

		const FText MessageLooping = FText::Format(
			NSLOCTEXT("AnimNotify", "NiagaraSystem_ShouldNotLoop", "Niagara system {0} used in anim notify for asset {1} is set to looping, but the slot is a one-shot (it won't be played to avoid leaking a component per notify)."),
			FText::AsCultureInvariant(NiagaraSystem->GetPathName()),
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
FString UScWAnimNotify_NiagaraSystem_IfVisible::GetNotifyName_Implementation() const // UAnimNotify
{
	if (NiagaraSystem)
	{
		return NiagaraSystem->GetName();
	}
	return Super::GetNotifyName_Implementation();
}

void UScWAnimNotify_NiagaraSystem_IfVisible::Notify(USkeletalMeshComponent* InMeshComponent, UAnimSequenceBase* InAnimationSequenceBase, const FAnimNotifyEventReference& InEventReference) // UAnimNotify
{
	if (UScWGameplayFunctionLibrary::IsComponentRenderedFor(InMeshComponent, InMeshComponent->GetOwner()))
	{
		SpawnedNiagaraSystem = SpawnNiagaraSystem(InMeshComponent, InAnimationSequenceBase);
	}
}
//~ End Notify

//~ Begin Niagara
UFXSystemComponent* UScWAnimNotify_NiagaraSystem_IfVisible::GetSpawnedNiagaraSystem()
{
	return SpawnedNiagaraSystem;
}

UFXSystemComponent* UScWAnimNotify_NiagaraSystem_IfVisible::SpawnNiagaraSystem(USkeletalMeshComponent* InMeshComponent, UAnimSequenceBase* InSequenceAnimationBase)
{
	UFXSystemComponent* OutComponent = nullptr;

	if (NiagaraSystem != nullptr)
	{
		if (NiagaraSystem->IsLooping())
		{
			return OutComponent;
		}
		if (bAttachParticles)
		{
			OutComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, InMeshComponent, AttachmentPoint, LocationOffset, RotationOffset, EAttachLocation::KeepRelativeOffset, true);
		}
		else
		{
			const FTransform MeshTransform = InMeshComponent->GetSocketTransform(AttachmentPoint);
			OutComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(InMeshComponent->GetWorld(), NiagaraSystem, MeshTransform.TransformPosition(LocationOffset), (MeshTransform.GetRotation() * RotationOffsetQuat).Rotator(), FVector::OneVector, true);
		}
		if (OutComponent != nullptr)
		{
			if (!FloatParameterToSet.IsNone())
			{
				OutComponent->SetFloatParameter(FloatParameterToSet, FloatParameterToSetValue);
			}
			OutComponent->SetUsingAbsoluteScale(bAbsoluteScale);
			OutComponent->SetRelativeScale3D_Direct(Scale);
		}
	}
	return OutComponent;
}
//~ End Niagara
