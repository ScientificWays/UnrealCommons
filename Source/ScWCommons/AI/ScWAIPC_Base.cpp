// G.o.o.P Studios

#include "AI/ATAAIPC_Base.h"

#include "AI/Services/BTS_GetPerceptionTarget.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"

UATAAIPC_Base::UATAAIPC_Base()
{
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SensesConfig.Add(SightConfig);

	SightConfig->SightRadius = 6000.0f;
	SightConfig->LoseSightRadius = 6100.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.0f;
	SightConfig->PointOfViewBackwardOffset = 50.0f;
	SightConfig->NearClippingRadius = 50.0f;
	SightConfig->SetMaxAge(10.0f);

	DominantSense = *SightConfig->GetSenseImplementation();

	UAISenseConfig_Hearing* HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	SensesConfig.Add(HearingConfig);

	HearingConfig->HearingRange = 10000.f;
	HearingConfig->SetMaxAge(15.0f);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;

	UAISenseConfig_Damage* DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	SensesConfig.Add(DamageConfig);

	DamageConfig->SetMaxAge(10.0f);
}

//~ Begin Initialize
void UATAAIPC_Base::BeginPlay() // UActorComponent
{
	Super::BeginPlay();
	OnPerceptionUpdated.AddDynamic(this, &UATAAIPC_Base::OnPerceptionUpdatedCallback);
}
//~ End Initialize

//~ Begin Services
void UATAAIPC_Base::RegisterGetPerceptionTargetService(UBTS_GetPerceptionTarget& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory)
{
	FPerceptionTargetServiceData NewData = { &InNode, &InOwnerTree, InNodeMemory };
	check(!PerceptionTargetServiceDataArray.Contains(NewData));
	PerceptionTargetServiceDataArray.Add(NewData);
}

void UATAAIPC_Base::UnRegisterGetPerceptionTargetService(uint8* InNodeMemory)
{
	FPerceptionTargetServiceData RemoveData = { nullptr, nullptr, InNodeMemory };
	check(PerceptionTargetServiceDataArray.Contains(RemoveData));
	PerceptionTargetServiceDataArray.RemoveSwap(RemoveData);
}
//~ End Services

//~ Begin Update
void UATAAIPC_Base::OnPerceptionUpdatedCallback(const TArray<AActor*>& InUpdatedActors)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]
	{
		if (this == nullptr)
		{
			return;
		}
		for (const FPerceptionTargetServiceData& SampleData : PerceptionTargetServiceDataArray)
		{
			if (SampleData.Node && SampleData.OwnerTree && SampleData.NodeMemory)
			{
				SampleData.Node->OnPerceptionUpdatedCallback(*SampleData.OwnerTree, SampleData.NodeMemory/*, InUpdatedActors*/);
			}
		}
	}));
}
//~ End Update
