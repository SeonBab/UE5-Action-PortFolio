#include "AI/Boss/LichAnimInstance.h"
#include "AI/Boss/Enums_Boss.h"
#include "AI/Boss/Lich.h"
#include "AI/Boss/DarkBall.h"
#include "AI/Boss/Tornado.h"
#include "Global/GlobalGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void ULichAnimInstance::AnimNotify_CreateDarkBall()
{
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Inst)
	{
		return;
	}

	TSubclassOf<UObject> DarkBall = Inst->GetSubClass(TEXT("DarkBall"));

	if (nullptr == DarkBall)
	{
		return;
	}

	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich)
	{
		return;
	}


	ADarkBall* SpawnDarkBall = GetWorld()->SpawnActor<ADarkBall>(DarkBall);

	if (nullptr == SpawnDarkBall)
	{
		return;
	}

	// �߻� ������ ������ �ֱ�
	Lich->SetDarkBall(SpawnDarkBall);

	// ��Ʈ�ѷ� ����
	AController* CurController = Lich->GetController();

	if (nullptr == CurController)
	{
		return;
	}

	SpawnDarkBall->SetCurController(CurController);

	// �ݸ��� �����ϱ�
	FName AttackType = Lich->GetAttackTypeTag();
	USphereComponent* SpherComponent = SpawnDarkBall->GetSphereComponent();

	if (nullptr == SpherComponent)
	{
		return;
	}

	SpherComponent->SetCollisionProfileName(AttackType);

	// DarkBallSoket ��ġ�� �����ϱ�
	USkeletalMeshComponent* LichMesh = Lich->GetMesh();
	FTransform Trans = LichMesh->GetSocketTransform(TEXT("DarkBallSoket"));
	FVector Pos = Trans.GetLocation();
	FRotator Rot = Lich->GetActorRotation();
	Lich->GetDarkBall()->SetActorLocation(Pos);
	Lich->GetDarkBall()->SetActorRotation(Rot);
}

void ULichAnimInstance::AnimNotify_DarkBallShot()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich)
	{
		return;
	}

	ADarkBall* Ball = Cast<ADarkBall>(Lich->GetDarkBall());

	if (nullptr == Ball)
	{
		return;
	}

	// �������� �߻��ϱ�
	Ball->SetSpeed(2000.f);
	Ball->SetDeathCheck(true);
	Lich->SetDarkBall(nullptr);
}

void ULichAnimInstance::AnimNotify_CreateTornado()
{
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Inst)
	{
		return;
	}

	TSubclassOf<UObject> Tornado = Inst->GetSubClass(TEXT("Tornado"));

	if (nullptr == Tornado)
	{
		return;
	}

	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich)
	{
		return;
	}

	ATornado* SpawnTornado = GetWorld()->SpawnActor<ATornado>(Tornado);

	if (nullptr == SpawnTornado)
	{
		return;
	}

	// ��Ʈ�ѷ� ����
	AController* CurController = Lich->GetController();

	if (nullptr == CurController)
	{
		return;
	}

	SpawnTornado->SetCurController(CurController);

	// �ݸ��� �����ϱ�
	FName AttackType = Lich->GetAttackTypeTag();
	UCapsuleComponent* CapsuleComponent = SpawnTornado->GetCapsuleComponent();

	if (nullptr == CapsuleComponent)
	{
		return;
	}

	CapsuleComponent->SetCollisionProfileName(AttackType);

	// Ÿ���� ��ġ��
	UBlackboardComponent* Blackboard = Lich->GetBlackboardComponent();
	
	if (nullptr == Blackboard)
	{
		return;
	}

	UObject* TargetObject = Blackboard->GetValueAsObject(TEXT("TargetActor"));

	if (nullptr == TargetObject)
	{
		return;
	}

	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (nullptr == TargetActor)
	{
		return;
	}

	FVector TargetPos = TargetActor->GetActorLocation();

	// ���� ���� �ִ�. ��� �÷���� �ұ�
	SpawnTornado->SetActorLocation(TargetPos);
}

void ULichAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void ULichAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OnMontageBlendingOut.AddDynamic(this, &ULichAnimInstance::MontageBlendingOut);
}

void ULichAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
}

void ULichAnimInstance::MontageBlendingOut(UAnimMontage* Anim, bool Inter)
{
	AGlobalCharacter* Character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == Character)
	{
		return;
	}

	if (Anim == GetAnimMontage(BossAnimState::Attack_Left) || Anim == GetAnimMontage(BossAnimState::Attack_Right) ||
		Anim == GetAnimMontage(BossAnimState::Attack_SpawnTornado))
	{
		Character->SetAnimState(BossAnimState::Idle);
		Montage_Play(AllAnimations[Character->GetAnimState()], 1.f);
	}
}

