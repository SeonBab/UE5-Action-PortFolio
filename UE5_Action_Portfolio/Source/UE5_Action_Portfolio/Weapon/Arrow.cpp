#include "Weapon/Arrow.h"
#include "Global/GlobalGameInstance.h"
#include "Global/GlobalCharacter.h"

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	ArrowSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ArrowSkeletalMesh->SetupAttachment(RootComponent);
	ArrowSkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"), true);

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
		
		float DirAngle = Dir.Rotation().Roll;
		float ForVecAngle = ForVec.Rotation().Roll;

		FRotator AddRot;
		FRotator SetRot;

		if (10.f < FMath::Abs(DirAngle - ForVecAngle))
		{
			AddRot += { Cross.X * 250 * _DeltaTime, 0, 0 };
			//SetRot.Roll = AddRot.Roll;
		}
		else
		{
			//SetRot.Roll = Dir.X;
		}

		DirAngle = Dir.Rotation().Pitch;
		ForVecAngle = ForVec.Rotation().Pitch;

		if (10.f < FMath::Abs(DirAngle - ForVecAngle))
		{
			AddRot += { 0, Cross.Y * 250 * _DeltaTime, 0 };
			//SetRot.Pitch = AddRot.Pitch;
		}
		else
		{
			//SetRot.Pitch = Dir.Y;
		}

		DirAngle = Dir.Rotation().Yaw;
		ForVecAngle = ForVec.Rotation().Yaw;

		if (10.f <= FMath::Abs(DirAngle - ForVecAngle))
		{
			AddRot += { 0, 0, Cross.Z * 250 * _DeltaTime };
			//SetRot.Yaw = AddRot.Yaw;
		}
		else
		{
			//SetRot.Yaw = Dir.Z;
		}

		// if에서 SetRot의 회전값을 각각 저장 else에서 if에 저장한 값 + else 해야하는 위치로 저장
		// 여기서 set하고 난 뒤 add를 하면 회전이 잘 되나??
		// Dir의 Rotation값을 넣으면 방향이 하늘을 바라본다 왤까?
		//SetRot = Dir.Rotation();
		//SetActorRotation(SetRot);
		
		AddRot.Euler();
		AddActorWorldRotation(AddRot);

		//FRotator Rot = FRotator::MakeFromEuler({ Cross.X * 250 * _DeltaTime, Cross.Y * 250 * _DeltaTime, Cross.Z * 250 * _DeltaTime });
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

