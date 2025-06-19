// Scientific Ways

#include "AI/Decorators/ScWBTD_CheckOcclusion.h"

#include "AI/ScWTypes_AI.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UScWBTD_CheckOcclusion::UScWBTD_CheckOcclusion()
{
	NodeName = TEXT("Check Occlusion");

	FromTargetKey.AllowNoneAsValue(true);
	FromTargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, FromTargetKey));
	FromTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, FromTargetKey), AActor::StaticClass());
	FromTargetKey.SelectedKeyName = FBlackboard::KeySelf;

	bCheckFromPawnViewLocation = true;

	ToTargetKey.AllowNoneAsValue(true);
	ToTargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, FromTargetKey));
	ToTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, FromTargetKey), AActor::StaticClass());
	ToTargetKey.SelectedKeyName = FScWBlackboard::KeyTargetActor;

	bCheckToPawnViewLocation = true;

	MaxCheckDistance = -1.0f;
	Offset = FRotator::ZeroRotator;

	TraceChannel = TraceTypeQuery_Visibility;
	DrawDebugTraceType = EDrawDebugTrace::None;
}

//~ Begin Decorator
FString UScWBTD_CheckOcclusion::GetStaticDescription() const // UBTNode
{
	FString FromDescription = TEXT("(invalid)");
	FString ToDescription = TEXT("in direction of view");
	FString OffsetDescription = TEXT("");

	if (FromTargetKey.IsSet())
	{
		FromDescription = FromTargetKey.SelectedKeyName.ToString();
	}
	if (ToTargetKey.IsSet())
	{
		ToDescription = TEXT("to ") + ToTargetKey.SelectedKeyName.ToString();
	}
	if (!Offset.IsNearlyZero(1e-3f))
	{
		OffsetDescription = FString::Printf(TEXT(" (with offset [P: %.1f, Y: %.1f, R: %.1f])"), Offset.Pitch, Offset.Yaw, Offset.Roll);
	}
	FString DistanceDescription = TEXT("");

	if (MaxCheckDistance > 0.0f)
	{
		DistanceDescription = FString::Printf(TEXT("\nMax check distance: %.1f"), MaxCheckDistance);
	}
	static const UEnum* TraceEnum = StaticEnum<ECollisionChannel>();
	FString TraceDescription = TraceEnum->GetNameStringByIndex(static_cast<int32>(UEngineTypes::ConvertToCollisionChannel(TraceChannel)));

	return FString::Printf(TEXT("%s\nFrom %s %s%s%s\nTrace channel: %s"), *Super::GetStaticDescription(), *FromDescription, *ToDescription, *OffsetDescription , *DistanceDescription, *TraceDescription);
}

void UScWBTD_CheckOcclusion::InitializeFromAsset(UBehaviorTree& InTreeAsset) // UBTNode
{
	Super::InitializeFromAsset(InTreeAsset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		FromTargetKey.ResolveSelectedKey(*BlackboardData);
		ToTargetKey.ResolveSelectedKey(*BlackboardData);
	}
}

bool UScWBTD_CheckOcclusion::CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const // UBTDecorator
{
	if (!FromTargetKey.IsSet())
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("%s FromTargetKey was not set! Decorator will always fail."), *GetName());
		return false;
	}
	if (AAIController* OwnerController = InOwnerTree.GetAIOwner())
	{
		if (UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent())
		{
			FVector FromTargetLocation = FVector::ZeroVector;
			FRotator FromTargetRotation = FRotator::ZeroRotator;

			TArray<AActor*> ActorsToIgnore;

			if (FromTargetKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
			{
				FromTargetLocation = BlackboardComponent->GetValueAsVector(FromTargetKey.SelectedKeyName);
			}
			else if (APawn* FromPawn = Cast<APawn>(BlackboardComponent->GetValueAsObject(FromTargetKey.SelectedKeyName)))
			{
				FromTargetLocation = bCheckFromPawnViewLocation ? FromPawn->GetPawnViewLocation() : FromPawn->GetActorLocation();
				FromTargetRotation = FromPawn->GetControlRotation();
				ActorsToIgnore.Add(FromPawn);
			}
			else if (AActor* FromActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(FromTargetKey.SelectedKeyName)))
			{
				FromTargetLocation = FromActor->GetActorLocation();
				FromTargetRotation = FromActor->GetActorRotation();
				ActorsToIgnore.Add(FromActor);
			}
			else
			{
				return true;
			}
			FromTargetLocation += FromTargetLocationOffset;

			FVector ToTargetLocation = FVector::ZeroVector;

			if (ToTargetKey.IsSet())
			{
				if (ToTargetKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
				{
					ToTargetLocation = BlackboardComponent->GetValueAsVector(ToTargetKey.SelectedKeyName);
				}
				else if (APawn* ToPawn = Cast<APawn>(BlackboardComponent->GetValueAsObject(FromTargetKey.SelectedKeyName)))
				{
					ToTargetLocation = bCheckToPawnViewLocation ? ToPawn->GetPawnViewLocation() : ToPawn->GetActorLocation();
					ActorsToIgnore.Add(ToPawn);
				}
				else if (AActor* ToActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(ToTargetKey.SelectedKeyName)))
				{
					ToTargetLocation = ToActor->GetActorLocation();
					ActorsToIgnore.Add(ToActor);
				}
				else
				{
					return true;
				}
				if (MaxCheckDistance > 0.0f)
				{
					ToTargetLocation = FromTargetLocation + (ToTargetLocation - FromTargetLocation).GetClampedToMaxSize(MaxCheckDistance);
				}
			}
			else
			{
				if (!FromTargetRotation.IsNearlyZero() && !Offset.IsNearlyZero())
				{
					FQuat FromTargetQuat = FQuat(FromTargetRotation);
					FQuat OffsetQuat = FQuat(Offset);

					FromTargetRotation = FRotator(FromTargetQuat * OffsetQuat);
					
					ToTargetLocation = FromTargetLocation + FromTargetRotation.Vector() * MaxCheckDistance;
				}
			}
			ToTargetLocation += ToTargetLocationOffset;

			FHitResult TraceHitResult;
			const bool bWasHit = UKismetSystemLibrary::LineTraceSingle(this, FromTargetLocation, ToTargetLocation, TraceChannel, false, ActorsToIgnore, DrawDebugTraceType, TraceHitResult, false, FLinearColor::Red, FLinearColor::Green, TickRate);

			return bWasHit;
		}
	}
	return true;
}
//~ End Decorator
