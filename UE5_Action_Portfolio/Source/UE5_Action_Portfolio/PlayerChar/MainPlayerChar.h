// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Global/Enums.h>
#include "MainPlayerChar.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AMainPlayerChar : public ACharacter
{
	GENERATED_BODY()

public:	
	AMainPlayerChar();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ZoomIn();
	void ZoomOut();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void JumpAction();

	void ChangeWeapon(FName _Weapon);

	void ChangeUnarm();
	void ChangeSword();

protected:
	virtual void BeginPlay() override;

public:
	class USpringArmComponent* TPSCameraSpringArmComponent;
	class UCameraComponent* TPSCameraComponent;
	float BaseTurnRate;
	float BaseLookUpRate;

	AnimPlayerState AniState;

	const struct FWeaponData* CurWeapon;
};
