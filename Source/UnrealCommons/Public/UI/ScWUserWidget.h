// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWUserWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALCOMMONS_API UScWUserWidget : public UUserWidget
{

	GENERATED_BODY()
	
public:

	UScWUserWidget(const FObjectInitializer& InObjectInitializer);

//~ Begin Initialize
public:
	virtual void NativePreConstruct() override; // UUserWidget
	virtual void NativeDestruct() override; // UUserWidget
//~ End Initialize

//~ Begin Owner
public:

	UPROPERTY(Category = "Owner", BlueprintReadOnly)
	TObjectPtr<class AScWPlayerController> OwnerPlayerController;
//~ End Owner

//~ Begin Input
public:

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	int32 InputMappingContextPriority;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	FModifyContextOptions InputMappingContextOptions;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	bool bShouldShowMouseCursor;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	bool bShouldBlockMovementInput;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	bool bShouldBlockLookInput;
//~ End Input
};
