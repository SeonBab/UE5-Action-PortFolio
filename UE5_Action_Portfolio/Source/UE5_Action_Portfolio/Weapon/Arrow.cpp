#include "Weapon/Arrow.h"
#include "Global/GlobalGameInstance.h"
#include "Global/GlobalCharacter.h"

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	ArrowScene = CreateDefaultSubobject<USceneComponent>(TEXT("Arrowcene"));
	ArrowSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ArrowSkeletalMesh->SetupAttachment(ArrowScene);
	ArrowSkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"), true);

	ArrowScene->SetupAttachment(RootComponent);
	ArrowSkeletalMesh->SetRelativeScale3D({ 1.3, 1.3, 1.3 });
}

void AArrow::ArrowBeginOverlap()
{

}

void AArrow::ArrowReRoad(ACharacter* _Character, FVector _JointPos, float _DeltaTime)
{
	// 화살이 생기고 어떻게 손에 붙어있다가 발사해야하는 타이밍에 날아가게 할 수 있을까
	// WeaponAction에서 액터 포인터를 저장하고 사용하다 발사하면서 저장한 포인터를 null로 바꿔가면서 써야하나?
	if (IsRotation == false)
	{
		FVector SetPos = _Character->GetMesh()->GetSocketLocation(TEXT("RightHandSoket"));
		
		SetActorLocation(SetPos);

		FVector Dir = _JointPos - SetPos;
		Dir.Normalize();

		FVector ForVec = GetActorForwardVector();
		ForVec.Normalize();

		FVector Cross = FVector::CrossProduct(ForVec, Dir);
		
		float DirAngle = Dir.Rotation().Yaw;
		float ForVecAngle = ForVec.Rotation().Yaw;

		FRotator SetRot;

		if (10.f <= FMath::Abs(DirAngle - ForVecAngle))
		{
			FRotator AddRot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 200 * _DeltaTime });
			AddActorWorldRotation(AddRot);
			SetRot.Yaw = Cross.Z * 200 * _DeltaTime;
		}
		else
		{
			SetRot.Yaw = Dir.Rotation().Yaw;
		}

		DirAngle = Dir.Rotation().Roll;
		ForVecAngle = ForVec.Rotation().Roll;

		if (10.f <= FMath::Abs(DirAngle - ForVecAngle))
		{
			FRotator AddRot = FRotator::MakeFromEuler({ Cross.X * 200 * _DeltaTime, 0, 0 });
			AddActorWorldRotation(AddRot);
			SetRot.Roll = Cross.X * 200 * _DeltaTime;
		}
		else
		{
			SetRot.Roll = Dir.Rotation().Roll;
		}

		//DirAngle = Dir.Rotation().Pitch;
		//ForVecAngle = ForVec.Rotation().Pitch;

		//if (10.f <= FMath::Abs(DirAngle - ForVecAngle))
		//{
		//	FRotator AddRot = FRotator::MakeFromEuler({ 0, Cross.Y * 200 * _DeltaTime, 0 });
		//	AddActorWorldRotation(AddRot);
		//	SetRot.Pitch = Cross.Y * 200 * _DeltaTime;
		//}
		//else
		//{
		//	SetRot.Pitch = Dir.Rotation().Pitch;
		//}

		SetActorRotation(SetRot);
	}
}

void AArrow::SetIsRotation(bool _Value)
{
	IsRotation = true;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
	UGlobalGameInstance* Instance = GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	ArrowSkeletalMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Arrow")));
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

