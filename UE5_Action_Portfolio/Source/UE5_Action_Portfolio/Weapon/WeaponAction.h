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

	void SetCurCharacter(ACharacter* _CurChar);

	MainCharacterAnimState* GetAnimState();
	void SetAnimState(MainCharacterAnimState _State);

public:
	virtual void WAndSButtonAction(float _Value);
	virtual void DAndAButtonAction(float _Value);
	virtual void RollorRunAction(float _Value);
	virtual void ShiftButtonAction();

protected:
	virtual void BeginPlay();

private:
	UPROPERTY()
	ACharacter* CurCharacter;
	UPROPERTY()
	MainCharacterAnimState AnimState = MainCharacterAnimState::Idle;

	bool IsForwardWalk;
	bool IsLeftdWalk;
	float PressTime;
};
