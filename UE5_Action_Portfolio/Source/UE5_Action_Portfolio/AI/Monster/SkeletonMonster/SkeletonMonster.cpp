#include "AI/Monster/SkeletonMonster/SkeletonMonster.h"
#include "Global/GlobalGameInstance.h"
#include "Global/Data/MonsterData.h"
#include "Global/Enums.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DamageEvents.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MainCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"

ASkeletonMonster::ASkeletonMonster()
{
    PrimaryActorTick.bCanEverTick = true;

    SetActorTypeTag(TEXT("Monster"));
    SetAttackTypeTag(TEXT("MonsterAttack"));
    Tags.Add(GetActorTypeTag());
    SetParrybool(false);

    DataName = "SkeletonMonster";

	MeleeDamage = 10.f;

	MeleeCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	MeleeCapsuleComponent->SetupAttachment(GetMesh(), TEXT("MeleeSocket"));
	MeleeCapsuleComponent->SetCollisionProfileName("NoCollision", true);
	MeleeCapsuleComponent->SetCapsuleHalfHeight(40.f);
	MeleeCapsuleComponent->SetCapsuleRadius(20.f);
}

void ASkeletonMonster::BeginPlay()
{
    Super::BeginPlay();

	MeleeCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ASkeletonMonster::MeleeBeginOverlap);
}

void ASkeletonMonster::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

float ASkeletonMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// PointDamage�� ���� �޾Ҵ�.
if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
{
	if (false == IsValid(EventInstigator))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	UAudioComponent* CurAudio = GetAudioComponent();

	if (nullptr == CurAudio)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == Audio"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	const FMonsterData* MonsterData = Instance->GetMonsterData(DataName);

	if (nullptr == MonsterData)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == MonsterData"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	if (0.f < GetHP() && 0.f < FinalDamage)
	{
		// ü�� ����
		SetHP(GetHP() - FinalDamage);

		// ü���� �������� �������� �ʾƾ��Ѵ�.
		if (0.f >= GetHP())
		{
			SetHP(0.f);
		}
	}

	APawn* EventInstigatorPawn = EventInstigator->GetPawn();

	if (false == IsValid(EventInstigatorPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	FVector HitDir = EventInstigatorPawn->GetActorLocation();
	HitDir.Z = 0;

	FVector CurPos = GetActorLocation();
	CurPos.Z = 0;

	FVector Dir = HitDir - CurPos;
	Dir.Normalize();

	SetActorRotation(Dir.Rotation());

	if (0.f < GetHP())
	{
		// ����
		if (AIAnimState::Idle == GetAnimState<AIAnimState>() || AIAnimState::Walk == GetAnimState<AIAnimState>())
		{
			SetAnimState(AIAnimState::GotHit);
		}

		// ��Ʈ ���� ť ����
		USoundCue* CurSoundCue = MonsterData->MapSoundCue.FindRef(AIAnimState::GotHit);

		if (nullptr == CurSoundCue)
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == CurSoundCue"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		CurAudio->SetSound(CurSoundCue);
	}
	else if (0.f >= GetHP())
	{

		// ����
		SetAnimState(AIAnimState::Death);

		// ���� ���� ť ����
		USoundCue* CurSoundCue = MonsterData->MapSoundCue.FindRef(AIAnimState::Death);

		if (nullptr == CurSoundCue)
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == CurSoundCue"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		CurAudio->SetSound(CurSoundCue);

		// ������ ���� �� ����
		UBlackboardComponent* Blackboard = GetBlackboardComponent();

		if (false == IsValid(Blackboard))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		Blackboard->SetValueAsBool(TEXT("IsDeath"), true);

		// �ݸ��� ����
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"), true);
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"), true);

		// �÷��̾��� ���� ����
		if (false == IsValid(EventInstigator))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		bool PlayerCheck = EventInstigator->GetPawn()->ActorHasTag("Player");

		if (true == PlayerCheck)
		{
			AMainCharacter* MainChar = Cast<AMainCharacter>(EventInstigator->GetPawn());

			if (nullptr == MainChar || false == MainChar->IsValidLowLevel())
			{
				UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
				return 0.f;
			}
		}
	}

	// ���� ���
	if (nullptr == CurAudio->GetSound())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == CurAudio->GetSound()"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	CurAudio->Play();

	return FinalDamage;
}

    return FinalDamage;
}

void ASkeletonMonster::MeleeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FPointDamageEvent DamageEvent;

	OtherActor->TakeDamage(MeleeDamage, DamageEvent, GetController(), this);
}

UCapsuleComponent* ASkeletonMonster::GetMeleeCapsuleComponent()
{
	return MeleeCapsuleComponent;
}

void ASkeletonMonster::AIInit()
{
	Super::AIInit();

	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	const FMonsterData* MonsterData = Instance->GetMonsterData(DataName);

	if (nullptr == MonsterData)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == MonsterData"), __FUNCTION__, __LINE__);
		return;
	}

	SetAllAnimation<AIAnimState>(MonsterData->MapAnimation);
	SetHP(MonsterData->HP);
	SetMaxHP(GetHP());
	SetAnimState<AIAnimState>(AIAnimState::Idle);

	UBlackboardComponent* CurBlackboardComponent = GetBlackboardComponent();

	if (false == IsValid(CurBlackboardComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CurBlackboardComponent->SetValueAsObject(TEXT("SelfActor"), this);
	CurBlackboardComponent->SetValueAsObject(TEXT("TargetActor"), nullptr);
	CurBlackboardComponent->SetValueAsObject(TEXT("NavPath"), nullptr);
	CurBlackboardComponent->SetValueAsFloat(TEXT("StateTime"), 0.f);
	CurBlackboardComponent->SetValueAsFloat(TEXT("MeleeAttackRange"), 125.f);
	CurBlackboardComponent->SetValueAsInt(TEXT("NavPathIndex"), 1);
	CurBlackboardComponent->SetValueAsVector(TEXT("SpawnPos"), GetActorLocation());
	CurBlackboardComponent->SetValueAsVector(TEXT("NavPathLastPos"), FVector::ZeroVector);
	CurBlackboardComponent->SetValueAsBool(TEXT("IsReturn"), false);
	CurBlackboardComponent->SetValueAsBool(TEXT("IsDeath"), false);
}
