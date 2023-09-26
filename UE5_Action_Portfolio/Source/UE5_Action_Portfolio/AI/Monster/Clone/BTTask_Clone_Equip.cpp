#include "AI/Monster/Clone/BTTask_Clone_Equip.h"

EBTNodeResult::Type UBTTask_Clone_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	CloneMonster->bUseControllerRotationYaw = true;

	EWeaponType CurWeaponType = WeaponComponent->GetWeaponType();

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (false == IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return EBTNodeResult::Failed;
	}

	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = CloneMonster->GetActorLocation();

	FVector Dir = TargetPos - CurPos;

	WeaponComponent->SetIsLockOn(true);

	int RandomInt = UGlobalGameInstance::MainRandom.RandRange(1, 3);

	if (800 <= Dir.Size() && 1 == RandomInt)
	{
		if (EWeaponType::Bow != CurWeaponType)
		{
			WeaponComponent->ChangeSetBow();
		}
	}
	else
	{
		if (EWeaponType::Sword != CurWeaponType)
		{
			WeaponComponent->ChangeSetSwordAndSheiled();
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Clone_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

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

	EWeaponType CurWeaponType = WeaponComponent->GetWeaponType();

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (false == IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = CloneMonster->GetActorLocation();

	FVector Dir = TargetPos - CurPos;

	if (EWeaponType::Bow == CurWeaponType)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else if (EWeaponType::Sword == CurWeaponType)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else if (EWeaponType::UnArmed == CurWeaponType)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
