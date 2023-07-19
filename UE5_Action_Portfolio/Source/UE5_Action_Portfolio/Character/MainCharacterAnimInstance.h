#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Global/Enums.h"
#include "MainCharacterAnimInstance.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void MontageEnd(UAnimMontage* Anim, bool Inter);
	UFUNCTION()
	void AnimNotify_RollStop();
	UFUNCTION()
	void AnimNotify_JumpStart();

protected:
	void NativeInitializeAnimation() override;
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaTime) override;

public:
	UPROPERTY()
	CharacterAnimState Animstate = CharacterAnimState::Idle;
	UPROPERTY()
	TMap<CharacterAnimState, UAnimMontage*> AllAnimations;

	// -가 되면 역재생?
	float AnimSpeed = 1.f;
};