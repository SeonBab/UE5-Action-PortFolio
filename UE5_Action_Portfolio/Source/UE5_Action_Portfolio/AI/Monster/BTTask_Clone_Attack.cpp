#include "AI/Monster/BTTask_Clone_Attack.h"

EBTNodeResult::Type UBTTask_Clone_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EWeaponType CurWeaponType = GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->GetWeaponType();

	// 근접 무기라면 공격 몇번 할지 횟수 구하기
	if (EWeaponType::Sword == CurWeaponType)
	{
		// 공격할지 방어할지 랜덤
		AttackOrBlock = UGlobalGameInstance::MainRandom.RandRange(0, 1) == 0 ? false : true;

		// false라면 공격
		if (false == AttackOrBlock)
		{
			MeleeAtackCount = UGlobalGameInstance::MainRandom.RandRange(0, 2);

			GetWeaponCharacter(OwnerComp)->CurWeaponAction->AttackAction();
		}
		// true라면 방어
		else if (true == AttackOrBlock)
		{
			BlockTime = UGlobalGameInstance::MainRandom.FRandRange(1.f, 3.f);
		}
	}
	else if (EWeaponType::Bow == CurWeaponType)
	{
		GetWeaponCharacter(OwnerComp)->CurWeaponAction->AttackAction();
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Clone_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	bool AttackCheck = GetWeaponCharacter(OwnerComp)->CurWeaponAction->GetAttackCheck();

	
	if (0.f < BlockTime)
	{
		GetWeaponCharacter(OwnerComp)->CurWeaponAction->AimorBlockAtion(1);
		BlockTime -= DeltaSeconds;
	}
	else if (true == AttackOrBlock && 0.f > BlockTime)
	{
		GetWeaponCharacter(OwnerComp)->CurWeaponAction->AimorBlockAtion(0);
	}
	else if (0 < MeleeAtackCount && false == AttackCheck)
	{
		--MeleeAtackCount;
		GetWeaponCharacter(OwnerComp)->CurWeaponAction->AttackAction();
	}

	CharacterAnimState CharAnim = (GetWeaponCharacter(OwnerComp)->CurWeaponAction->GetAnimState());

	if (CharacterAnimState::Dizzy == CharAnim || CharacterAnimState::Death == CharAnim)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else if (CharacterAnimState::LockOnIdle == CharAnim)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
