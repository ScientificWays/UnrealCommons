// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ATAUserWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ATA_DREAMCOMETRUE_API UATAUserWidget : public UUserWidget
{

	GENERATED_BODY()
	
public:

	UATAUserWidget(const FObjectInitializer& InObjectInitializer);

//~ Begin Initialize
public:
	virtual void NativePreConstruct() override; // UUserWidget
	virtual void NativeDestruct() override; // UUserWidget
//~ End Initialize

//~ Begin Owner
public:

	UPROPERTY(Category = "Owner", BlueprintReadOnly)
	TObjectPtr<class AATAPlayerController> OwnerPlayerController;
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
