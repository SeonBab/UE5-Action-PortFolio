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
	// ȭ���� ����� ��� �տ� �پ��ִٰ� �߻��ؾ��ϴ� Ÿ�ֿ̹� ���ư��� �� �� ������
	// WeaponAction���� ���� �����͸� �����ϰ� ����ϴ� �߻��ϸ鼭 ������ �����͸� null�� �ٲ㰡�鼭 ����ϳ�?
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

		// if���� SetRot�� ȸ������ ���� ���� else���� if�� ������ �� + else �ؾ��ϴ� ��ġ�� ����
		// ���⼭ set�ϰ� �� �� add�� �ϸ� ȸ���� �� �ǳ�??
		// Dir�� Rotation���� ������ ������ �ϴ��� �ٶ󺻴� �ͱ�?
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

