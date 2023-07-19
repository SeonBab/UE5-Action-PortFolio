#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Character.h"
#include "Global/Enums.h"
#include "WeaponAction.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UWeaponAction : public UObject
{
	GENERATED_BODY()
	
public:
	UWeaponAction();
	
	void Tick(float _DeltaTime) {};
	void SetCurCharacter(ACharacter* _CurChar);
	void SetCharacterAirControl(float _Value);

	CharacterAnimState* GetAnimState();
	void SetAnimState(CharacterAnimState _State);

	void IsRollMoveToFalse();
	void ChangeWeapon(FName _Weapon);
	void ChangeSetUnArmed();
	void ChangeSetBow();
	void ChangeSetSwordAndSheiled();
	void PressSpaceBarCkeck(float _DeltaTime);

	virtual void WAndSButtonAction(float _Value);
	virtual void DAndAButtonAction(float _Value);
	virtual void RollorRunAction(float _Value);
	virtual void ShiftButtonAction();

	virtual void BeginPlay();

public:
	// 캐릭터
	UPROPERTY()
	ACharacter* CurCharacter;
	CharacterAnimState AnimState = CharacterAnimState::Idle;

	// 캐릭터 움직임 상태
	bool IsForwardWalk;
	bool IsLeftWalk;
	bool IsRollMove;
	bool IsWalkJump;
	bool IsRunJump;
	bool PressSpacebar;
	float PressSpacebarTime;
	int RunCount;
};
