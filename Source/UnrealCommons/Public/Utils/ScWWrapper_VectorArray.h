// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWWrapper_VectorArray.generated.h"

/**
* 
*/
UCLASS(Blueprintable, meta = (DisplayName = "[ScW] Wrapper (VectorArray)"))
class UNREALCOMMONS_API UScWWrapper_VectorArray : public UObject
{
	GENERATED_BODY()

public:	

	UScWWrapper_VectorArray();
	
//~ Begin Get
public:

	UFUNCTION(Category = "Get", BlueprintCallable, BlueprintPure, meta = (DefaultToSelf = "InOuter"))
	static UScWWrapper_VectorArray* CreateVectorArrayWrapper(UObject* InOuter, const TArray<FVector>& InVectorArray);
//~ End Get

//~ Begin Data
public:

	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite)
	TArray<FVector> VectorArray;
//~ End Data
};
