// Scientific Ways

#pragma once

#include "UnrealCommons.h"

//#include "ScWTypes_Containers.generated.h"

template<typename ElementType>
struct TArraySetPair
{
private:
	TArray<ElementType> Array;
	TSet<ElementType> Set;
public:

	FORCEINLINE const TArray<ElementType>& GetConstArray() const { return Array; }
	FORCEINLINE const TSet<ElementType>& GetConstSet() const { return Set; }

	FORCEINLINE bool Add(const ElementType& InElement)
	{
		if (Contains(InElement))
		{
			return false;
		}
		else
		{
			Set.Add(InElement);
			Array.Add(InElement);
			ensure(Array.Num() == Set.Num());
			return true;
		}
	}

	FORCEINLINE bool Remove(const ElementType& InElement)
	{
		if (Contains(InElement))
		{
			Set.Remove(InElement);
			Array.Remove(InElement);
			ensure(Array.Num() == Set.Num());
			return true;
		}
		else
		{
			return false;
		}
	}

	FORCEINLINE bool Replace(const ElementType& InPrevElement, const ElementType& InNewElement)
	{
		if (Contains(InPrevElement))
		{
			Remove(InPrevElement);
			Add(InNewElement);
			return true;
		}
		else
		{
			return false;
		}
	}

	FORCEINLINE void AddFromOther(const TArraySetPair<ElementType>& InOther)
	{
		for (const ElementType& SampleOtherItem : InOther.Array)
		{
			Add(SampleOtherItem);
		}
	}

	FORCEINLINE void RemoveFromOther(const TArraySetPair<ElementType>& InOther)
	{
		for (const ElementType& SampleOtherItem : InOther.Array)
		{
			Remove(SampleOtherItem);
		}
	}

	FORCEINLINE void AddHeadTo(int32 InDesiredElementsNum, TArraySetPair<ElementType>& InOutOther, const bool bInPopElements = false)
	{
		int32 FirstIndex = 0;
		int32 LastIndex = FMath::Min(InDesiredElementsNum, Array.Num()) - 1;

		for (int32 SampleIndex = FirstIndex; SampleIndex <= LastIndex; ++SampleIndex)
		{
			InOutOther.Add(Array[SampleIndex]);
		}
		if (bInPopElements)
		{
			RemoveFromOther(InOutOther);
		}
	}

	FORCEINLINE void AddTailTo(int32 InDesiredElementsNum, TArraySetPair<ElementType>& InOutOther, const bool bInPopElements = false)
	{
		int32 FirstIndex = Array.Num() - 1;
		int32 LastIndex = Array.Num() - FMath::Min(InDesiredElementsNum, Array.Num());

		for (int32 SampleIndex = FirstIndex; SampleIndex >= LastIndex; --SampleIndex)
		{
			InOutOther.Add(Array[SampleIndex]);
		}
		if (bInPopElements)
		{
			RemoveFromOther(InOutOther);
		}
	}

	FORCEINLINE int32 Num() const { return Array.Num(); }
	FORCEINLINE bool IsEmpty() const { return Array.IsEmpty(); }
	FORCEINLINE bool Contains(const ElementType& InElement) const { return Set.Contains(InElement); }

	FORCEINLINE void Empty(int32 InSlack = 0)
	{
		Array.Empty(InSlack);
		Set.Empty(InSlack);
	}

	FORCEINLINE ElementType Pop(EAllowShrinking InAllowShrinking = EAllowShrinking::Default)
	{
		ElementType PoppedItem = Array.Pop(InAllowShrinking);
		Set.Remove(PoppedItem);
		return PoppedItem;
	}
};

template<typename KeyType, typename ValueType>
struct TMirroredMapPair
{
private:
	TMap<KeyType, ValueType> KeyValueMap;
	TMap<ValueType, KeyType> ValueKeyMap;
public:

	FORCEINLINE const TMap<KeyType, ValueType>& GetConstKeyValueMap() const { return KeyValueMap; }
	FORCEINLINE const TMap<ValueType, KeyType>& GetConstValueKeyMap() const { return ValueKeyMap; }

	FORCEINLINE bool AddPair(const KeyType& InKey, const ValueType& InValue)
	{
		ensure(ContainsKey(InKey) == ContainsValue(InValue));

		if (ContainsKey(InKey))
		{
			return false;
		}
		KeyValueMap.Add(InKey, InValue);
		ValueKeyMap.Add(InValue, InKey);
		ensure(KeyValueMap.Num() == ValueKeyMap.Num());
		return true;
	}

	FORCEINLINE bool RemoveByKey(const KeyType& InKey)
	{
		if (ContainsKey(InKey))
		{
			ValueKeyMap.Remove(KeyValueMap[InKey]);
			KeyValueMap.Remove(InKey);
			ensure(KeyValueMap.Num() == ValueKeyMap.Num());
			return true;
		}
		return false;
	}

	FORCEINLINE bool RemoveByValue(const ValueType& InValue)
	{
		if (ContainsValue(InValue))
		{
			KeyValueMap.Remove(ValueKeyMap[InValue]);
			ValueKeyMap.Remove(InValue);
			ensure(KeyValueMap.Num() == ValueKeyMap.Num());
			return true;
		}
		return false;
	}

	FORCEINLINE bool ReplaceKey(const KeyType& InPrevKey, const KeyType& InNewKey)
	{
		if (ContainsKey(InPrevKey))
		{
			if (ContainsKey(InNewKey))
			{
				Swap(ValueKeyMap[KeyValueMap[InPrevKey]], ValueKeyMap[KeyValueMap[InNewKey]]);
				Swap(KeyValueMap[InPrevKey], KeyValueMap[InNewKey]);
			}
			else
			{
				ValueType ValueAtPrevKey = KeyValueMap[InPrevKey];

				RemoveByValue(ValueAtPrevKey);
				AddPair(ValueAtPrevKey, InNewKey);
				ensure(KeyValueMap.Num() == ValueKeyMap.Num());
			}
			return true;
		}
		return false;
	}

	FORCEINLINE bool ReplaceValue(const ValueType& InPrevValue, const ValueType& InNewValue)
	{
		if (ContainsValue(InPrevValue))
		{
			if (ContainsValue(InNewValue))
			{
				Swap(KeyValueMap[ValueKeyMap[InPrevValue]], KeyValueMap[ValueKeyMap[InNewValue]]);
				Swap(ValueKeyMap[InPrevValue], ValueKeyMap[InNewValue]);
			}
			else
			{
				KeyType KeyAtPrevValue = ValueKeyMap[InPrevValue];

				RemoveByKey(KeyAtPrevValue);
				AddPair(KeyAtPrevValue, InNewValue);
				ensure(KeyValueMap.Num() == ValueKeyMap.Num());
			}
			return true;
		}
		return false;
	}

	FORCEINLINE ValueType* FindByKey(const KeyType& InKey) const { return const_cast<ValueType*>(KeyValueMap.Find(InKey)); }
	FORCEINLINE KeyType* FindByValue(const ValueType& InValue) const { return const_cast<KeyType*>(ValueKeyMap.Find(InValue)); }

	FORCEINLINE ValueType& FindRefByKey(const KeyType& InKey, const ValueType& InDefaultValue) const
	{
		if (const ValueType* FoundValuePtr = KeyValueMap.Find(InKey))
		{
			return const_cast<ValueType&>(*FoundValuePtr);
		}
		return const_cast<ValueType&>(InDefaultValue);
	}

	FORCEINLINE KeyType& FindRefByValue(const ValueType& InValue, const KeyType& InDefaultKey) const
	{
		if (const KeyType* FoundKeyPtr = ValueKeyMap.Find(InValue))
		{
			return const_cast<KeyType&>(*FoundKeyPtr);
		}
		return const_cast<KeyType&>(InDefaultKey);
	}

	FORCEINLINE int32 Num() const { return KeyValueMap.Num(); }
	FORCEINLINE bool IsEmpty() const { return KeyValueMap.IsEmpty(); }

	FORCEINLINE bool ContainsKey(const KeyType& InKey) const { return KeyValueMap.Contains(InKey); }
	FORCEINLINE bool ContainsValue(const ValueType& InValue) const { return ValueKeyMap.Contains(InValue); }

	FORCEINLINE void Empty(int32 InSlack = 0)
	{
		KeyValueMap.Empty(InSlack);
		ValueKeyMap.Empty(InSlack);
	}
};
