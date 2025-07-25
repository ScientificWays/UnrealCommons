// Scientific Ways

#include "Gameplay/Handhelds/ScWHandheldData.h"

UScWHandheldData::UScWHandheldData()
{
	DisplayName = FText::FromString(TEXT("Common Handheld"));

	bCanDrop = false;
	bDropOnDeath = false;

	FP_OwnerMeshAttachmentSocketName = TEXT("Hand_R");
	TP_OwnerMeshAttachmentSocketName = TEXT("Hand_RSocket");
}
