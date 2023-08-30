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
	void SetAttackCheck(bool _Value);
	void SetIsLockOn(bool _Value);
	bool GetIsAimOn();
	bool GetIsRollMove();
	void SetAttackType(FName _AttackType);
	void SetIsBlock(bool _Value);
	bool GetIsBlock();
	void SetIsParry(bool _Value);
	bool GetIsParry();
	bool GetIsInvincibility();
	void ChangeCollisionAttackType();
	void ChangeNoCollision();
	void ArrowSpawn();
	AArrow* GetReadyArrow();
	void SetnullReadyArrow();

	UFUNCTION(BlueprintCallable)
	CharacterAnimState GetAnimState();
	UFUNCTION(BlueprintCallable)
	int GetAnimStateToInt();
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
	void ParryAction();

	UFUNCTION(BlueprintCallable)
	float GetMoveXValue();
	UFUNCTION(BlueprintCallable)
	float GetMoveYValue();
	UFUNCTION(BlueprintCallable)
	bool GetIsLockOn();
	UFUNCTION(BlueprintCallable)
	bool GetIsMove();
	UFUNCTION(BlueprintCallable)
	bool GetLockOnCheck();
	void SetLockOnCheck(bool _Value);

	void GotHit(FVector _Value);
	void Death();

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
	bool IsInvincibility = false;
	bool IsWalkJump = false;
	bool IsRunJump = false;
	bool IsLockOn = false;
	bool IsAimOn = false;
	bool IsBlock = false;
	bool IsParry = false;
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
