#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalAnimInstance.h"
#include "CloneMonsterAnimInstance.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UCloneMonsterAnimInstance : public UGlobalAnimInstance
{
	GENERATED_BODY()
	
public:
	void MontageBlendingOut(UAnimMontage* Anim, bool Inter) override;

	UFUNCTION()
	void AnimNotify_RollStop();
	UFUNCTION()
	void AnimNotify_JumpStart();
	UFUNCTION()
	void AnimNotify_ChangeWeapon();
	UFUNCTION()
	void AnimNotify_AttackCheck();
	UFUNCTION()
	void AnimNotify_AimorBlockCheck();
	UFUNCTION()
	void AnimNotify_ArrowReadyCheck();
	UFUNCTION()
	void AnimNotify_ChordToHand();
	UFUNCTION()
	void AnimNotify_ReRoad();
	UFUNCTION()
	void AnimNotify_StartAttack();
	UFUNCTION()
	void AnimNotify_EndAttack();
	UFUNCTION()
	void AnimNotify_Death();
	UFUNCTION()
	void AnimNotify_ParryOnOff();

protected:
	void NativeInitializeAnimation() override;
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaTime) override;
};
