// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include <Global/Enums.h>
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5_ACTION_PORTFOLIO_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void MontageEnd(UAnimMontage* Anim, bool Inter);

protected:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaTime) override;

public:
	AnimPlayerState Animstate = AnimPlayerState::Idle;
	TMap<AnimPlayerState, UAnimMontage*> AllAnimations;
};
