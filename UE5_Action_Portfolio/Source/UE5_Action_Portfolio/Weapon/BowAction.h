// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponAction.h"
#include "BowAction.generated.h"

/**
 * 
 */
UCLASS()
class UE5_ACTION_PORTFOLIO_API UBowAction : public UWeaponAction
{
	GENERATED_BODY()
	
public:
	virtual void WAndSButtonAction(float _Value) override;
	virtual void DAndAButtonAction(float _Value) override;
	virtual void RollorRunAction(float _Value) override;
	virtual void ShiftButtonAction() override;

private:
	// 활 데이터
};
