#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalCharacter.h"
#include "Global/Enums.h"
#include "WeaponCharacter.generated.h"

class UWeaponAction;
// UObject�� ��ӹްų� ������Ʈ�� ��������� ����ؼ� ����
UCLASS()
class UE5_ACTION_PORTFOLIO_API AWeaponCharacter : public AGlobalCharacter
{
	GENERATED_BODY()

public:
	AWeaponCharacter();
	UFUNCTION(BlueprintCallable)
	UWeaponAction* GetCurWeaponAction();
	FVector GetBowJointLocation();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent) override;
	UFUNCTION()
	void WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void BowChordMove();

public:
	UPROPERTY()
	UWeaponAction* CurWeaponAction;

	// ���� �� ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* UnArmedWeaponMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* BowWeaponMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* SwordWeaponMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* ShieldWeaponMesh;

	// ���� ���� ���� ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* BackBowWeaponMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* BackSwordWeaponMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* BackShieldWeaponMesh;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator FootRotatorLeft;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator FootRotatorRight;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float HipOffset;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FootOffsetLeft;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FootOffsetRight;
	float CurCapsuleSize;
};
