// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponAction.h"
#include "Components/CapsuleComponent.h"

UWeaponAction::UWeaponAction()
{
	//CurCharacter = CreateDefaultSubobject<ACharacter>(TEXT("Weapon"));
	
}

void UWeaponAction::SetCurCharacter(ACharacter* _CurChar)
{
	CurCharacter = _CurChar;
	UE_LOG(LogTemp, Log, TEXT("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
}

void UWeaponAction::BeginPlay()
{
	
}



void UWeaponAction::WAndSButtonAction(float _Value)
{
	if (nullptr != CurCharacter->Controller && _Value != 0.0f)
	{
		const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

		CurCharacter->AddMovementInput(Direction, _Value);
	}
}

void UWeaponAction::DAndAButtonAction(float _Value)
{
	if (nullptr != CurCharacter->Controller && _Value != 0.0f)
	{
		const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		CurCharacter->AddMovementInput(Direction, _Value);
	}
}

void UWeaponAction::ShiftButtonAction()
{
	CurCharacter->Jump();
}
