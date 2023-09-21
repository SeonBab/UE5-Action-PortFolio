#include "AI/Monster/BTTask_Clone_Attack.h"

EBTNodeResult::Type UBTTask_Clone_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACloneMonster* CloneMonster = GetCloneMonster(OwnerComp);

	if (false == IsValid(CloneMonster))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return EBTNodeResult::Failed;
	}

	UWeaponComponent* WeaponComponent = CloneMonster->GetWeaponComponent();

	if (false == IsValid(WeaponComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return EBTNodeResult::Failed;
	}

	EWeaponType CurWeaponType = WeaponComponent->GetWeaponType();

	// 근접 무기라면 공격 몇번 할지 횟수 구하기
	if (EWeaponType::Sword == CurWeaponType)
	{
		// 공격할지 방어할지 랜덤
		AttackOrBlock = UGlobalGameInstance::MainRandom.RandRange(0, 1) == 0 ? false : true;

		// false라면 공격
		if (false == AttackOrBlock)
		{
			MeleeAtackCount = UGlobalGameInstance::MainRandom.RandRange(0, 2);

			WeaponComponent->AttackAction();
		}
		// true라면 방어
		else if (true == AttackOrBlock)
		{
			BlockTime = UGlobalGameInstance::MainRandom.FRandRange(1.f, 3.f);
		}
	}
	else if (EWeaponType::Bow == CurWeaponType)
	{
		WeaponComponent->AttackAction();
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Clone_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACloneMonster* CloneMonster = GetCloneMonster(OwnerComp);

	if (false == IsValid(CloneMonster))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UWeaponComponent* WeaponComponent = CloneMonster->GetWeaponComponent();

	if (false == IsValid(WeaponComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	bool AttackCheck = WeaponComponent->GetAttackCheck();

	
	if (0.f < BlockTime)
	{
		WeaponComponent->AimorBlockAtion(1);
		BlockTime -= DeltaSeconds;
	}
	else if (true == AttackOrBlock && 0.f > BlockTime)
	{
		WeaponComponent->AimorBlockAtion(0);
	}
	else if (0 < MeleeAtackCount && false == AttackCheck)
	{
		--MeleeAtackCount;
		WeaponComponent->AttackAction();
	}

	CharacterAnimState CharAnim = static_cast<CharacterAnimState>(CloneMonster->GetAnimState());

	if (CharacterAnimState::Dizzy == CharAnim || CharacterAnimState::Death == CharAnim)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else if (CharacterAnimState::LockOnIdle == CharAnim)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
