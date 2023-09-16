#include "AI/Boss/BTTask_Boss_Rotation.h"

EBTNodeResult::Type UBTTask_Boss_Rotation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::InProgress;
}

void UBTTask_Boss_Rotation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

	if (nullptr == Blackboard || false == Blackboard->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}
	
	UObject* TargetObject = Blackboard->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);
	
	if (nullptr == TargetActor || false == TargetActor->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}
	
	AGlobalCharacter* CurCharacter = GetGlobalCharacter(OwnerComp);
	 
	if (nullptr == CurCharacter || false == CurCharacter->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}
	
	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = CurCharacter->GetActorLocation();
	
	TargetPos.Z = 0;
	CurPos.Z = 0;
	
	FVector Dir = TargetPos - CurPos;
	Dir.Normalize();
	
	FVector OtherForward = CurCharacter->GetActorForwardVector();
	OtherForward.Normalize();
	
	FVector Cross = FVector::CrossProduct(OtherForward, Dir);
	
	float Angle0 = Dir.Rotation().Yaw;
	float Angle1 = OtherForward.Rotation().Yaw;
	
	BossAnimState AnimState = CurCharacter->GetAnimState<BossAnimState>();
	
	if (5.f <= FMath::Abs(Angle0 - Angle1))
	{
		FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 500.f * DeltaSeconds });
		CurCharacter->AddActorWorldRotation(Rot);
	}
	else if (5.f >= FMath::Abs(Angle0 - Angle1) && BossAnimState::GotHit != AnimState)
	{
		FRotator Rot = Dir.Rotation();
		CurCharacter->SetActorRotation(Rot);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
