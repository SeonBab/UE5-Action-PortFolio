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

	// 콜리전 설정
	FName AttackType = Lich->GetAttackTypeTag();

	CapsuleComponent->SetCollisionProfileName(AttackType);

	// 나이아가라 설정
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

	// 콜리전 설정
	CapsuleComponent->SetCollisionProfileName(TEXT("NoCollision"));

	// 나이아가라 설정

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

	// 발사 전까지 가지고 있기
	Lich->SetDarkBall(SpawnDarkBall);

	// 컨트롤러 설정
	AController* CurController = Lich->GetController();

	if (nullptr == CurController)
	{
		return;
	}

	SpawnDarkBall->SetCurController(CurController);

	// 콜리전 변경하기
	FName AttackType = Lich->GetAttackTypeTag();
	USphereComponent* SpherComponent = SpawnDarkBall->GetSphereComponent();

	if (nullptr == SpherComponent)
	{
		return;
	}

	SpherComponent->SetCollisionProfileName(AttackType);

	// DarkBallSoket 위치로 변경하기
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

	// 전방으로 발사하기
	FVector ForwardVec = Lich->GetActorForwardVector();
	Ball->ShotDarkBall(ForwardVec);

	// 리치가 가지고 있는 다크볼 지우기
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

	// 컨트롤러 설정
	AController* CurController = Lich->GetController();

	if (nullptr == CurController)
	{
		return;
	}

	SpawnTornado->SetCurController(CurController);

	// 변경할 콜리전 타입 넘겨주기
	FName AttackType = Lich->GetAttackTypeTag();
	SpawnTornado->SetAttackType(AttackType);

	// 타겟의 위치로
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

	// 타겟 액터 설정
	SpawnTornado->SetTargetActor(TargetActor);

	// 페이즈2부터 속도가 빨라지게
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

