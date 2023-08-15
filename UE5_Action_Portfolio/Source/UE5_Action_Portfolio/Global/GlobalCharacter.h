#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Global/Enums.h"
#include "GlobalCharacter.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AGlobalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGlobalCharacter();
	UFUNCTION(BlueprintCallable)
	class UWeaponAction* GetCurWeaponAction();
	UFUNCTION()
	void WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	FVector GetBowJointLocation();
	void SetActorTypeTag(FName _Tag);
	void SetAttackTypeTag(FName _Tag);
	FName GetAttackTypeTag();
	void SetHP(float _HP);
	float GetHP();

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
	CharacterAnimState* AnimState;

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
	FName ActorTypeTag;
	FName AttackTypeTag;
	float HP;
};
