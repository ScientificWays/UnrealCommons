// Scientific Ways

#include "UI/ScWUserWidget.h"

#include "Framework/ScWPlayerState.h"
#include "Framework/ScWPlayerController.h"

#include "Gameplay/ScWGameplayFunctionLibrary.h"

UScWUserWidget::UScWUserWidget(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
	InputMappingContextPriority = 1;
}

//~ Begin Initialize
void UScWUserWidget::NativePreConstruct() // UUserWidget
{
	if (!IsDesignTime())
	{
		APlayerController* OwningPlayer = GetOwningPlayer();

		if (OwningPlayer && InputMappingContext)
		{
			UScWGameplayFunctionLibrary::AddEnhancedInputMappingContextTo(OwningPlayer, InputMappingContext, InputMappingContextPriority, InputMappingContextOptions);
		}
		OwnerPlayerController = Cast<AScWPlayerController>(OwningPlayer);

		if (OwnerPlayerController)
		{
			if (bShouldShowMouseCursor)
			{
				OwnerPlayerController->AddShowMouseCursorSource(this);
			}
			if (bShouldBlockMovementInput)
			{
				OwnerPlayerController->AddMovementInputBlockSource(this);
			}
			if (bShouldBlockLookInput)
			{
				OwnerPlayerController->AddLookInputBlockSource(this);
			}
		}
		if (OwningPlayer)
		{
			OwnerPlayerState = OwningPlayer->GetPlayerState<AScWPlayerState>();
		}
	}
	Super::NativePreConstruct();
}

void UScWUserWidget::NativeDestruct() // UUserWidget
{
	if (!IsDesignTime())
	{
		APlayerController* OwningPlayer = GetOwningPlayer();

		if (OwningPlayer && InputMappingContext)
		{
			UScWGameplayFunctionLibrary::RemoveEnhancedInputMappingContextFrom(OwningPlayer, InputMappingContext, InputMappingContextOptions);
		}
		if (OwnerPlayerController)
		{
			if (bShouldShowMouseCursor)
			{
				OwnerPlayerController->RemoveShowMouseCursorSource(this);
			}
			if (bShouldBlockMovementInput)
			{
				OwnerPlayerController->RemoveMovementInputBlockSource(this);
			}
			if (bShouldBlockLookInput)
			{
				OwnerPlayerController->RemoveLookInputBlockSource(this);
			}
		}
	}
	Super::NativeDestruct();
}
//~ End Initialize
