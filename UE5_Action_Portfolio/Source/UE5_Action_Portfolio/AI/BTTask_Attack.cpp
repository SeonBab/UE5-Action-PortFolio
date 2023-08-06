#include "AI/BTTask_Attack.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EWeaponType CurWeaponType = GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->GetWeaponType();

	// 근접 무기라면 공격 몇번 할지 횟수 구하기
	if (EWeaponType::Sword == CurWeaponType)
	{
		// 공격할지 방어할지 랜덤
		AttackOrBlock = UGlobalGameInstance::MainRandom.RandRange(0, 1) == 0 ? false : true;

		// false라면 공격
		if (false == AttackOrBlock)
		{
			MeleeAtackCount = UGlobalGameInstance::MainRandom.RandRange(0, 2);

			GetGlobalCharacter(OwnerComp)->CurWeaponAction->AttackAction();
		}
		// true라면 방어
		else if (true == AttackOrBlock)
		{
			BlockTime = UGlobalGameInstance::MainRandom.FRandRange(1.f, 3.f);
		}
	}
	else if (EWeaponType::Bow == CurWeaponType)
	{
		GetGlobalCharacter(OwnerComp)->CurWeaponAction->AttackAction();
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	CharacterAnimState* CharAnim = (GetGlobalCharacter(OwnerComp)->CurWeaponAction->GetAnimState());

	bool AttackCheck = GetGlobalCharacter(OwnerComp)->CurWeaponAction->GetAttackCheck();

	
	if (0.f < BlockTime)
	{
		GetGlobalCharacter(OwnerComp)->CurWeaponAction->AimorBlockAtion(1);
		BlockTime -= DeltaSeconds;
	}
	else if (true == AttackOrBlock && 0.f > BlockTime)
	{
		GetGlobalCharacter(OwnerComp)->CurWeaponAction->AimorBlockAtion(0);
	}
	else if (0 < MeleeAtackCount && false == AttackCheck)
	{
		--MeleeAtackCount;
		GetGlobalCharacter(OwnerComp)->CurWeaponAction->AttackAction();
	}

	if (CharacterAnimState::LockOnIdle == *CharAnim)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
