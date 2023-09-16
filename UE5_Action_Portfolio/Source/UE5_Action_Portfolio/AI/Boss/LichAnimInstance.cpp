#include "AI/Boss/LichAnimInstance.h"
#include "AI/Boss/Enums_Boss.h"
#include "AI/Boss/Lich.h"
#include "AI/Boss/DarkBall.h"
#include "AI/Boss/Tornado.h"
#include "AI/Boss/Frostbolt.h"
#include "Global/GlobalGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

void ULichAnimInstance::AnimNotify_MeleeStart()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich || false == Lich->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}


	UCapsuleComponent* CapsuleComponent = Lich->GetMeleeCapsuleComponent();

	if (nullptr == CapsuleComponent || false == CapsuleComponent->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	// 콜리전 설정
	FName AttackType = Lich->GetAttackTypeTag();

	CapsuleComponent->SetCollisionProfileName(AttackType);

	// 나이아가라 설정
	UNiagaraComponent* NiagaraComponent = Lich->GetMeleeNiagaraComponent();

	if (nullptr == NiagaraComponent || false == NiagaraComponent->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Inst || false == Inst->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}
	
	UNiagaraSystem* TrailWind = Inst->GetNiagaraAsset(TEXT("TrailWind"));

	if (nullptr == TrailWind || false == TrailWind->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}
	
	NiagaraComponent->SetAsset(TrailWind);

}

void ULichAnimInstance::AnimNotify_MeleeEnd()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich || false == Lich->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	UCapsuleComponent* CapsuleComponent = Lich->GetMeleeCapsuleComponent();

	if (nullptr == CapsuleComponent || false == CapsuleComponent->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	// 콜리전 설정
	CapsuleComponent->SetCollisionProfileName(TEXT("NoCollision"));

	// 나이아가라 설정

	UNiagaraComponent* NiagaraComponent = Lich->GetMeleeNiagaraComponent();

	if (nullptr == NiagaraComponent || false == NiagaraComponent->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	NiagaraComponent->SetAsset(nullptr);
}

void ULichAnimInstance::AnimNotify_CreateDarkBall()
{
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Inst || false == Inst->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	TSubclassOf<UObject> DarkBall = Inst->GetSubClass(TEXT("DarkBall"));

	if (nullptr == DarkBall || false == DarkBall->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich || false == Lich->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	ADarkBall* SpawnDarkBall = GetWorld()->SpawnActor<ADarkBall>(DarkBall);

	if (nullptr == SpawnDarkBall || false == SpawnDarkBall->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
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

	if (nullptr == SpherComponent || false == SpherComponent->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
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

	if (nullptr == Lich || false == Lich->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	ADarkBall* Ball = Cast<ADarkBall>(Lich->GetDarkBall());

	if (nullptr == Ball || false == Ball->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
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

	if (nullptr == Inst || false == Inst->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	TSubclassOf<UObject> Tornado = Inst->GetSubClass(TEXT("Tornado"));

	if (nullptr == Tornado || false == Tornado->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich || false == Lich->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	ATornado* SpawnTornado = GetWorld()->SpawnActor<ATornado>(Tornado);

	if (nullptr == SpawnTornado || false == SpawnTornado->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	// 컨트롤러 설정
	AController* CurController = Lich->GetController();

	if (nullptr == CurController || false == CurController->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	SpawnTornado->SetCurController(CurController);

	// 변경할 콜리전 타입 넘겨주기
	FName AttackType = Lich->GetAttackTypeTag();
	SpawnTornado->SetAttackType(AttackType);

	// 타겟의 위치로
	AActor* TargetActor = Lich->GetTargetActor();
	AGlobalCharacter* TargetCharacter = Cast<AGlobalCharacter>(TargetActor);

	if (nullptr == TargetCharacter || false == TargetCharacter->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	FVector TargetPos = TargetActor->GetActorLocation();

	UCapsuleComponent* TornadoCapsuleComponent = SpawnTornado->GetCapsuleComponent();

	if (nullptr == TornadoCapsuleComponent || false == TornadoCapsuleComponent->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}


	float TargetCapsuleSize = TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float PlusZPos = TornadoCapsuleComponent->GetScaledCapsuleHalfHeight();
	PlusZPos = PlusZPos - TargetCapsuleSize;

	FVector SetPos({ TargetPos.X, TargetPos.Y, TargetPos.Z + PlusZPos });

	SpawnTornado->SetActorLocation(SetPos);

	// 타겟 액터 설정
	SpawnTornado->SetTargetActor(TargetActor);

	// 페이즈2부터 EQS를 사용해 랜덤한 위치에 2개가 더 생성
}

void ULichAnimInstance::AnimNotify_FrostboltSpawn()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich || false == Lich->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	// 페이즈 가져와서 몇개의 볼트를 소환할지 계산 1페이즈 3개, 2페이즈 5개
	int LichPhase = Lich->GetPhase();
	TArray<AActor*>& FrostboltArray = Lich->GetFrostboltArray();
	int FrostboltCount = FrostboltArray.Num();

	if (1 == LichPhase && 3 <= FrostboltCount)
	{
		return;
	}

	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Inst || false == Inst->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	TSubclassOf<UObject> Frostbolt = Inst->GetSubClass(TEXT("Frostbolt"));

	if (nullptr == Frostbolt || false == Frostbolt->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	// Frostbolt 스폰
	AFrostbolt* SpawnFrostbolt = GetWorld()->SpawnActor<AFrostbolt>(Frostbolt);

	if (nullptr == SpawnFrostbolt || false == SpawnFrostbolt->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	// 발사 전까지 가지고 있기
	FrostboltArray.Emplace(SpawnFrostbolt);

	// 컨트롤러 설정
	AController* CurController = Lich->GetController();

	if (nullptr == CurController || false == CurController->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	SpawnFrostbolt->SetCurController(CurController);

	// 콜리전 변경하기
	FName AttackType = Lich->GetAttackTypeTag();

	USphereComponent* SpherComponent = SpawnFrostbolt->GetSphereComponent();

	if (nullptr == SpherComponent || false == SpherComponent->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	SpherComponent->SetCollisionProfileName(AttackType);

	// FrostboltSoket 위치로 변경하기
	USkeletalMeshComponent* LichMesh = Lich->GetMesh();

	if (nullptr == LichMesh || false == LichMesh->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	FString FrostboltSoketStr = "FrostboltSoket";
	FString FrostboltIndexStr = FString::FromInt(FrostboltCount);
	FString FrostboltSoketNameIndex = FrostboltSoketStr + FrostboltIndexStr;
	FName FindFrostboltSoket = FName(*FrostboltSoketNameIndex);

	FTransform Trans = LichMesh->GetSocketTransform(FindFrostboltSoket);
	FVector Pos = Trans.GetLocation();
	FRotator Rot = Lich->GetActorRotation();
	SpawnFrostbolt->SetActorLocation(Pos);
	SpawnFrostbolt->SetActorRotation(Rot);

	// 타겟 액터 설정

	AActor* TargetActor = Lich->GetTargetActor();

	if (nullptr == TargetActor || false == TargetActor->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	SpawnFrostbolt->SetTargetActor(TargetActor);
}

void ULichAnimInstance::AnimNotify_FrostboltShot()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich || false == Lich->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	TArray<AActor*>& FrostboltArray = Lich->GetFrostboltArray();

	for (int i = 0; i < FrostboltArray.Num(); i++)
	{
		if (nullptr == FrostboltArray[i])
		{
			continue;
		}

		AFrostbolt* Frostbolt = Cast<AFrostbolt>(FrostboltArray[i]);

		if (nullptr == Frostbolt || false == Frostbolt->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
			continue;
		}

		// 가지고있던 방향대로 발사하기
		Frostbolt->ShotFrostbolt();

		// 리치가 가지고 있는 프로스트볼트 포인트 값 초기화
		Lich->SetNullFrostboltArray(i);

		if (i + 1 == FrostboltArray.Num())
		{
			FrostboltArray.Empty();
		}

		break;
	}

	
}

void ULichAnimInstance::AnimNotify_Death()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (nullptr == Lich || false == Lich->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
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

	if (nullptr == Character || false == Character->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
		return;
	}

	if (Anim == GetAnimMontage(BossAnimState::Attack_Left) || Anim == GetAnimMontage(BossAnimState::Attack_Right) ||
		Anim == GetAnimMontage(BossAnimState::Attack_SpawnTornado) || Anim == GetAnimMontage(BossAnimState::Attack_FrostboltShot) ||
		Anim == GetAnimMontage(BossAnimState::GotHit) || Anim == GetAnimMontage(BossAnimState::Visible))
	{
		Character->SetAnimState(BossAnimState::Idle);
		Montage_Play(AllAnimations[Character->GetAnimState()], 1.f);
	}
}

