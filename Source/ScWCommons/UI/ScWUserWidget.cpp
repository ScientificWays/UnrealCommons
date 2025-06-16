// G.o.o.P Studios

#include "UI/ATAUserWidget.h"

#include "Player/ATAPlayerController.h"

UATAUserWidget::UATAUserWidget(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
	InputMappingContextPriority = 1;
}

//~ Begin Initialize
void UATAUserWidget::NativePreConstruct() // UUserWidget
{
	if (!IsDesignTime())
	{
		OwnerPlayerController = GetOwningPlayer<AATAPlayerController>();

		if (InputMappingContext)
		{
			if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					InputSubsystem->AddMappingContext(InputMappingContext, InputMappingContextPriority, InputMappingContextOptions);
				}
			}
		}
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
	}
	Super::NativePreConstruct();
}

void UATAUserWidget::NativeDestruct() // UUserWidget
{
	if (!IsDesignTime())
	{
		if (InputMappingContext)
		{
			if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					InputSubsystem->RemoveMappingContext(InputMappingContext, InputMappingContextOptions);
				}
			}
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
