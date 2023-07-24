#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Global/Enums.h"
#include "WeaponAction.generated.h"

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

	CharacterAnimState* GetAnimState();
	void SetAnimState(CharacterAnimState _State);
	// 무기 장착하고 해제하는 매쉬 해야함
	void IsRollMoveToFalse();
	void ChangeWeapon(FName _Weapon);
	void ChangeSetUnArmed();
	void ChangeSetBow();
	void ChangeSetSwordAndSheiled();
	void PressSpaceBarCkeckAndRoll(float _DeltaTime);

	virtual void WAndSButtonAction(float _Value);
	virtual void DAndAButtonAction(float _Value);
	virtual void RollorRunAction(float _Value);
	virtual void ShiftButtonAction();
	virtual void AttackAction();
	virtual void AimorBlockAtion(float _Value);


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
	bool IsLeftWalk = false;
	bool IsRollMove = false;
	bool IsWalkJump = false;
	bool IsRunJump = false;
	bool PressSpacebar = false;
	bool AttackCheck = false;
	bool ArrowReady = false;
	bool EarlyArrowCheck = false;
	float PressSpacebarTime = 0.f;
	const float RunCount = 0.8f;
	const float WalkSpeed = 500.f;
	const float RunSpeed = 700.f;
	const float AimorBlockSpeed = 250.f;
};
