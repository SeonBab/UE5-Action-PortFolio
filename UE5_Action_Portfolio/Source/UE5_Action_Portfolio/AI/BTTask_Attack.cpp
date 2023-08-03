#include "AI/BTTask_Attack.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EWeaponType CurWeaponType = GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->GetWeaponType();

	// 근접 무기라면 공격 몇번 할지 횟수 구하기
	if (EWeaponType::Sword == CurWeaponType)
	{
		MeleeAtackCount = UGlobalGameInstance::MainRandom.RandRange(0, 2);
	}

	GetGlobalCharacter(OwnerComp)->CurWeaponAction->AttackAction();

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	CharacterAnimState CharAnim = (*(GetGlobalCharacter(OwnerComp)->CurWeaponAction->GetAnimState()));

	bool AttackCheck = GetGlobalCharacter(OwnerComp)->CurWeaponAction->GetAttackCheck();

	if (0 < MeleeAtackCount && false == AttackCheck)
	{
		--MeleeAtackCount;
		GetGlobalCharacter(OwnerComp)->CurWeaponAction->AttackAction();
	}

	if (CharacterAnimState::LockOnIdle == CharAnim)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
