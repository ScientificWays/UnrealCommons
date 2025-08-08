// Scientific Ways

#include "Utils/ScWWrapper_VectorArray.h"

UScWWrapper_VectorArray::UScWWrapper_VectorArray()
{
	
}

//~ Begin Get
UScWWrapper_VectorArray* UScWWrapper_VectorArray::CreateVectorArrayWrapper(UObject* InOuter, const TArray<FVector>& InVectorArray)
{
	ThisClass* OutWrapper = NewObject<ThisClass>(InOuter);
	OutWrapper->VectorArray = InVectorArray;
	return OutWrapper;
}
//~ End Get
