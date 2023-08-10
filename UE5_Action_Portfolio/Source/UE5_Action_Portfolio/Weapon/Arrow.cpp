#include "Weapon/Arrow.h"
#include "Global/GlobalGameInstance.h"
#include "Global/GlobalCharacter.h"

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	ArrowScene = CreateDefaultSubobject<USceneComponent>(TEXT("Arrowcene"));
	ArrowSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	SetRootComponent(ArrowScene);

	ArrowSkeletalMesh->SetupAttachment(ArrowScene);
	ArrowSkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"), true);
	ArrowSkeletalMesh->SetRelativeScale3D({ 1.3, 1.3, 1.3 });

	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	//ProjectileMovement->bRotationFollowsVelocity = true;

	InitialLifeSpan = 0.f;
}

void AArrow::ArrowBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 공격 충돌 처리
	//OtherActor->TakeDamage(10.f, );

	Destroy();
}

void AArrow::ArrowReRoad(ACharacter* _Character, FVector _JointPos, float _DeltaTime)
{
	if (IsLocationAndRotation == true)
	{
		FVector SetPos = _Character->GetMesh()->GetSocketLocation(TEXT("RightHandSoket"));
		
		SetActorLocation(SetPos);

		FVector Dir = _JointPos - SetPos;
		Dir.Normalize();

		//FVector ForVec = GetActorForwardVector();
		//ForVec.Normalize();

		//FVector Cross = FVector::CrossProduct(ForVec, Dir);
		//
		//float DirAngle = Dir.Rotation().Yaw;
		//float ForVecAngle = ForVec.Rotation().Yaw;

		//FRotator SetRot;

		//if (10.f <= FMath::Abs(DirAngle - ForVecAngle))
		//{
		//	FRotator AddRot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 800 * _DeltaTime });
		//	AddActorWorldRotation(AddRot);
		//}
		//else
		//{
		//	SetRot.Yaw = Dir.Rotation().Yaw;
		//}

		//FVector ForVec = GetActorUpVector();
		//ForVec.Normalize();

		//FVector Cross = FVector::CrossProduct(ForVec, Dir);

		//DirAngle = Dir.Rotation().Pitch;
		//ForVecAngle = ForVec.Rotation().Pitch;

		//if (10.f <= FMath::Abs(DirAngle - ForVecAngle))
		//{
		//	FRotator AddRot = FRotator::MakeFromEuler({ 0, Cross.Y * 800 * _DeltaTime, 0 });
		//	AddActorWorldRotation(AddRot);
		//}
		//else
		//{
		//	SetRot.Pitch = Dir.Rotation().Pitch;
		//}

		//SetActorRotation(SetRot);

		SetActorRotation(Dir.Rotation());
	}
}

void AArrow::SetIsLocationAndRotation(bool _Value)
{
	IsLocationAndRotation = false;
}

void AArrow::ArrowChangeCollision(FName _FName)
{
	ArrowSkeletalMesh->SetCollisionProfileName(_FName, true);
}

void AArrow::FireInDirection(FVector _FVector)
{
	ProjectileMovement->SetUpdatedComponent(ArrowSkeletalMesh);
	ProjectileMovement->InitialSpeed = 4000.f;

	FVector ShotVector = GetActorForwardVector();
	ShotVector.Z += 0.1f;

	ProjectileMovement->Velocity = _FVector * ProjectileMovement->InitialSpeed;
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

	ArrowSkeletalMesh->OnComponentBeginOverlap.AddDynamic(this, &AArrow::ArrowBeginOverlap);
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

