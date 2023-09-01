#include "AI/Boss/LichAnimInstance.h"
#include "AI/Boss/Enums_Boss.h"
#include "AI/Boss/Lich.h"
#include "AI/Boss/DarkBall.h"
#include "AI/Boss/Tornado.h"
#include "Global/GlobalGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

void ULichAnimInstance::AnimNotify_MeleeStart()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich)
	{
		return;
	}

	UCapsuleComponent* CapsuleComponent = Lich->GetMeleeCapsuleComponent();

	if (nullptr == CapsuleComponent)
	{
		return;
	}

	// �ݸ��� ����
	FName AttackType = Lich->GetAttackTypeTag();

	CapsuleComponent->SetCollisionProfileName(AttackType);

	// ���̾ư��� ����
	UNiagaraComponent* NiagaraComponent = Lich->GetMeleeNiagaraComponent();

	if (nullptr == NiagaraComponent)
	{
		return;
	}

	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Inst)
	{
		return;
	}
	
	UNiagaraSystem* TrailWind = Inst->GetNiagaraAsset(TEXT("TrailWind"));

	if (nullptr == TrailWind)
	{
		return;
	}
	
	NiagaraComponent->SetAsset(TrailWind);

}

void ULichAnimInstance::AnimNotify_MeleeEnd()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich)
	{
		return;
	}

	UCapsuleComponent* CapsuleComponent = Lich->GetMeleeCapsuleComponent();

	if (nullptr == CapsuleComponent)
	{
		return;
	}

	// �ݸ��� ����
	CapsuleComponent->SetCollisionProfileName(TEXT("NoCollision"));

	// ���̾ư��� ����

	UNiagaraComponent* NiagaraComponent = Lich->GetMeleeNiagaraComponent();

	if (nullptr == NiagaraComponent)
	{
		return;
	}

	NiagaraComponent->SetAsset(nullptr);
}

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
	FTransform Trans = LichMesh->GetSocketTransform(TEXT("DarkBallSocket"));
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
	FVector ForwardVec = Lich->GetActorForwardVector();
	Ball->ShotDarkBall(ForwardVec);

	// ��ġ�� ������ �ִ� ��ũ�� �����
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

	// ������ �ݸ��� Ÿ�� �Ѱ��ֱ�
	FName AttackType = Lich->GetAttackTypeTag();
	SpawnTornado->SetAttackType(AttackType);

	// Ÿ���� ��ġ��
	AActor* TargetActor = Lich->GetTargetActor();

	if (nullptr == TargetActor)
	{
		return;
	}

	FVector TargetPos = TargetActor->GetActorLocation();

	UCapsuleComponent* CapsuleComponent = SpawnTornado->GetCapsuleComponent();

	if (nullptr == CapsuleComponent)
	{
		return;
	}

	float PlusZPos = CapsuleComponent->GetScaledCapsuleHalfHeight();
	PlusZPos -= TargetPos.Z;

	FVector SetPos({ TargetPos.X, TargetPos.Y, TargetPos.Z + PlusZPos });

	SpawnTornado->SetActorLocation(SetPos);

	// Ÿ�� ���� ����
	SpawnTornado->SetTargetActor(TargetActor);

	// ������2���� �ӵ��� ��������
	int LichPhase = Lich->GetPhase();

	if (2 <= LichPhase)
	{
		float TornadoSpeed = SpawnTornado->GetSpeed();
		TornadoSpeed *= 2.f;
		SpawnTornado->SetSpeed(TornadoSpeed);
	}
}

void ULichAnimInstance::AnimNotify_FrostboltSpawn()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich)
	{
		return;
	}

	int LichPhase = Lich->GetPhase();

	Lich->GetFrostboltArray();
}

void ULichAnimInstance::AnimNotify_FrostboltShot()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich)
	{
		return;
	}


}

void ULichAnimInstance::AnimNotify_Death()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich || false == Lich->IsValidLowLevel())
	{
		return;
	}

	Lich->Destroy();
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
		Anim == GetAnimMontage(BossAnimState::Attack_SpawnTornado) || Anim == GetAnimMontage(BossAnimState::Attack_FrostboltShot) ||
		Anim == GetAnimMontage(BossAnimState::GotHit))
	{
		Character->SetAnimState(BossAnimState::Idle);
		Montage_Play(AllAnimations[Character->GetAnimState()], 1.f);
	}
}

