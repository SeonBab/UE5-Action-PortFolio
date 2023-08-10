#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Global/Enums.h"
#include "WeaponAction.generated.h"

class AArrow;

UCLASS()
class UE5_ACTION_PORTFOLIO_API UWeaponAction : public UObject
{
	GENERATED_BODY()
	
public:
	UWeaponAction();
	virtual void BeginPlay();
	
	void Tick(float _DeltaTime);
	void SetCurCharacter(ACharacter* _CurChar);
	void SetCharacterAirControl(float _Value);
	UFUNCTION(BlueprintCallable)
	EWeaponType GetWeaponType();
	bool GetAttackCheck();
	void SetLockOnCheck(bool _Value);
	bool GetLockOnCheck();
	bool GetIsAimOn();
	void SetAttackType(FName _AttackType);
	void ChangeCollisionAttackType();
	void ChangeNoCollision();
	void ArrowSpawn();
	AArrow* GetReadyArrow();
	void SetnullReadyArrow();
	UFUNCTION()
	void AttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	CharacterAnimState* GetAnimState();
	void SetAnimState(CharacterAnimState _State);
	void IsRollMoveToFalse();
	void ChangeWeapon(FName _Weapon);
	void ChangeSetUnArmed();
	void ChangeSetBow();
	void ChangeSetSwordAndSheiled();
	void PressSpaceBarCkeckAndRoll(float _DeltaTime);
	bool LockOnAfterRun();

	void WAndSButtonAction(float _Value);
	void DAndAButtonAction(float _Value);
	void RollorRunAction(float _Value);
	void ShiftButtonAction();
	void AttackAction();
	void AimorBlockAtion(float _Value);

	UFUNCTION(BlueprintCallable)
	float GetMoveXValue();
	UFUNCTION(BlueprintCallable)
	float GetMoveYValue();
	UFUNCTION(BlueprintCallable)
	bool GetIsLockOn();
	UFUNCTION(BlueprintCallable)
	bool GetIsMove();

public:
	// 캐릭터
	UPROPERTY()
	ACharacter* CurCharacter;
	CharacterAnimState AnimState = CharacterAnimState::Idle;

	// 무기
	EWeaponType WeaponType = EWeaponType::UnArmed;

	// 캐릭터 움직임 상태
	bool BowToSwordAndSheiled = false;
	bool SwordAndSheiledToBow = false;
	bool IsForwardWalk = false; 
	bool IsBackwardWalk = false;
	bool IsLeftWalk = false;
	bool IsRightWalk = false;
	bool IsRollMove = false;
	bool IsWalkJump = false;
	bool IsRunJump = false;
	bool IsLockOn = false;
	bool IsAimOn = false;
	bool PressSpacebar = false;
	bool AttackCheck = false;
	bool ArrowReady = false;
	bool EarlyArrowCheck = false;
	bool LockOnCheck = false;
	float PressSpacebarTime = 0.f;
	float LockOnAfterRunTime = 0.f;
	const float LockOnAfterRunCount = 2.f;
	const float RunCount = 0.8f;
	const float WalkSpeed = 450.f;
	const float LockOnSpeed = 400.f;
	const float RunSpeed = 700.f;
	const float AimorBlockSpeed = 250.f;
	float MoveXValue = 0.f;
	float MoveYValue = 0.f;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AArrow> ArrowClass;
	UPROPERTY()
	AArrow* ReadyArrow = nullptr;
	FName AttackType;
};
