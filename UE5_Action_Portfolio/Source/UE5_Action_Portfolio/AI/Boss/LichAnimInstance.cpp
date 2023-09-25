#include "AI/Boss/LichAnimInstance.h"
#include "Global/Enums.h"
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

	if (false == IsValid(Lich))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}


	UCapsuleComponent* CapsuleComponent = Lich->GetMeleeCapsuleComponent();

	if (false == IsValid(CapsuleComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// �ݸ��� ����
	FName AttackType = Lich->GetAttackTypeTag();

	CapsuleComponent->SetCollisionProfileName(AttackType);

	// ���̾ư��� ����
	UNiagaraComponent* NiagaraComponent = Lich->GetMeleeNiagaraComponent();

	if (false == IsValid(NiagaraComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}
	
	UNiagaraSystem* TrailWind = Instance->GetNiagaraAsset(TEXT("TrailWind"));

	if (false == IsValid(TrailWind))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}
	
	NiagaraComponent->SetAsset(TrailWind);

}

void ULichAnimInstance::AnimNotify_MeleeEnd()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (false == IsValid(Lich))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UCapsuleComponent* CapsuleComponent = Lich->GetMeleeCapsuleComponent();

	if (false == IsValid(CapsuleComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// �ݸ��� ����
	CapsuleComponent->SetCollisionProfileName(TEXT("NoCollision"));

	// ���̾ư��� ����

	UNiagaraComponent* NiagaraComponent = Lich->GetMeleeNiagaraComponent();

	if (false == IsValid(NiagaraComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	NiagaraComponent->SetAsset(nullptr);
}

void ULichAnimInstance::AnimNotify_CreateDarkBall()
{
	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	TSubclassOf<UObject> DarkBall = Instance->GetSubClass(TEXT("DarkBall"));

	if (false == IsValid(DarkBall))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (false == IsValid(Lich))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	ADarkBall* SpawnDarkBall = GetWorld()->SpawnActor<ADarkBall>(DarkBall);

	if (false == IsValid(SpawnDarkBall))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// �߻� ������ ������ �ֱ�
	Lich->SetDarkBall(SpawnDarkBall);

	// ��Ʈ�ѷ� ����
	AController* CurController = Lich->GetController();

	if (false == IsValid(CurController))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	SpawnDarkBall->SetCurController(CurController);

	// �ݸ��� �����ϱ�
	FName AttackType = Lich->GetAttackTypeTag();
	USphereComponent* SpherComponent = SpawnDarkBall->GetSphereComponent();

	if (false == IsValid(SpherComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
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

	if (false == IsValid(Lich))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	ADarkBall* Ball = Cast<ADarkBall>(Lich->GetDarkBall());

	if (false == IsValid(Ball))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
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
	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	TSubclassOf<UObject> Tornado = Instance->GetSubClass(TEXT("Tornado"));

	if (false == IsValid(Tornado))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (false == IsValid(Lich))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	ATornado* SpawnTornado = GetWorld()->SpawnActor<ATornado>(Tornado);

	if (false == IsValid(SpawnTornado))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// ��Ʈ�ѷ� ����
	AController* CurController = Lich->GetController();

	if (false == IsValid(CurController))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	SpawnTornado->SetCurController(CurController);

	// ������ �ݸ��� Ÿ�� �Ѱ��ֱ�
	FName AttackType = Lich->GetAttackTypeTag();
	SpawnTornado->SetAttackType(AttackType);

	// Ÿ���� ��ġ��
	AActor* TargetActor = Lich->GetTargetActor();
	AGlobalCharacter* TargetCharacter = Cast<AGlobalCharacter>(TargetActor);

	if (false == IsValid(TargetCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	FVector TargetPos = TargetActor->GetActorLocation();

	UCapsuleComponent* TornadoCapsuleComponent = SpawnTornado->GetCapsuleComponent();

	if (false == IsValid(TornadoCapsuleComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}


	float TargetCapsuleSize = TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float PlusZPos = TornadoCapsuleComponent->GetScaledCapsuleHalfHeight();
	PlusZPos = PlusZPos - TargetCapsuleSize;

	FVector SetPos({ TargetPos.X, TargetPos.Y, TargetPos.Z + PlusZPos });

	SpawnTornado->SetActorLocation(SetPos);

	// Ÿ�� ���� ����
	SpawnTornado->SetTargetActor(TargetActor);

	// ������2���� EQS�� ����� ������ ��ġ�� 2���� �� ����
}

void ULichAnimInstance::AnimNotify_FrostboltSpawn()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (false == IsValid(Lich))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// ������ �����ͼ� ��� ��Ʈ�� ��ȯ���� ��� 1������ 3��, 2������ 5��
	int LichPhase = Lich->GetPhase();
	TArray<AActor*>& FrostboltArray = Lich->GetFrostboltArray();
	int FrostboltCount = FrostboltArray.Num();

	if (1 == LichPhase && 3 <= FrostboltCount)
	{
		return;
	}

	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	TSubclassOf<UObject> Frostbolt = Instance->GetSubClass(TEXT("Frostbolt"));

	if (false == IsValid(Frostbolt))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// Frostbolt ����
	AFrostbolt* SpawnFrostbolt = GetWorld()->SpawnActor<AFrostbolt>(Frostbolt);

	if (false == IsValid(SpawnFrostbolt))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// �߻� ������ ������ �ֱ�
	FrostboltArray.Emplace(SpawnFrostbolt);

	// ��Ʈ�ѷ� ����
	AController* CurController = Lich->GetController();

	if (false == IsValid(CurController))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	SpawnFrostbolt->SetCurController(CurController);

	// �ݸ��� �����ϱ�
	FName AttackType = Lich->GetAttackTypeTag();

	USphereComponent* SpherComponent = SpawnFrostbolt->GetSphereComponent();

	if (false == IsValid(SpherComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	SpherComponent->SetCollisionProfileName(AttackType);

	// FrostboltSoket ��ġ�� �����ϱ�
	USkeletalMeshComponent* LichMesh = Lich->GetMesh();

	if (false == IsValid(LichMesh))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
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

	// Ÿ�� ���� ����

	AActor* TargetActor = Lich->GetTargetActor();

	if (false == IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	SpawnFrostbolt->SetTargetActor(TargetActor);
}

void ULichAnimInstance::AnimNotify_FrostboltShot()
{
	ALich* Lich = Cast<ALich>(GetOwningActor());

	if (false == IsValid(Lich))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
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

		if (false == IsValid(Frostbolt))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			continue;
		}

		// �������ִ� ������ �߻��ϱ�
		Frostbolt->ShotFrostbolt();

		// ��ġ�� ������ �ִ� ���ν�Ʈ��Ʈ ����Ʈ �� �ʱ�ȭ
		Lich->SetNullFrostboltArray(i);

		if (i + 1 == FrostboltArray.Num())
		{
			FrostboltArray.Empty();
		}

		break;
	}

	
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

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	BossAnimState AnimState = static_cast<BossAnimState>(Character->GetAnimState());

	if (BossAnimState::Death == AnimState)
	{
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

