#include "AI/Paladin.h"
#include "Global/GlobalGameInstance.h"
#include "Global/Data/MonsterData.h"
#include "BehaviorTree/BlackboardComponent.h"

void APaladin::BeginPlay()
{
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Inst)
	{
		return;
	}

	CurMonsterData = Inst->GetMonsterData(DataName);
	// 비헤이비어 트리 Set하기

	Super::BeginPlay();

	GetBlackboardComponent()->SetValueAsString(TEXT("TargetTag"), TEXT("Player"));
	GetBlackboardComponent()->SetValueAsFloat(TEXT("SearchRange"), 1500.f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("HP"), CurMonsterData->HP);
}
