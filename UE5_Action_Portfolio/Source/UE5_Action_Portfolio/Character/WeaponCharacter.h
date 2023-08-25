#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalCharacter.h"
#include "Global/Enums.h"
#include "WeaponCharacter.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AWeaponCharacter : public AGlobalCharacter
{
	GENERATED_BODY()

public:
	AWeaponCharacter();
	UFUNCTION(BlueprintCallable)
	class UWeaponAction* GetCurWeaponAction();
	UFUNCTION()
	void WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	FVector GetBowJointLocation();
	TTuple<float, FVector> IKFootLineTrace(FName _Socket, float _TraceDis);
	FRotator NormalToRotator(FVector _Vector);
	void UpdateFootRotation(float _DeltaTime, FRotator _NormalToRotatorValue, FRotator* _FootRotatorValue, float _InterpSpeed);
	void UpdateCapsuleHalfHeight(float _DeltaTime, float _HipsShifs, bool _ResetDefault);
	void UpdateFootOffset(float _DeltaTime, float _TargetValue, float* _EffectorValue, float _InterpSpeed);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* _PlayerInputComponent) override;
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

private:
	void BowChordMove();

public:
	UPROPERTY()
	class UWeaponAction* CurWeaponAction;

	// 장착 한 무기
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* UnArmedWeaponMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* BowWeaponMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* SwordWeaponMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	USkeletalMeshComponent* ShieldWeaponMesh;

	// 장착 하지 않은 무기
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
