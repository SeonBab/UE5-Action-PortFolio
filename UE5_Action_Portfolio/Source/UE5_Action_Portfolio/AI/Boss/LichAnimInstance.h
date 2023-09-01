#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalAnimInstance.h"
#include "LichAnimInstance.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API ULichAnimInstance : public UGlobalAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void AnimNotify_MeleeStart();
	UFUNCTION()
	void AnimNotify_MeleeEnd();
	UFUNCTION()
	void AnimNotify_CreateDarkBall();
	UFUNCTION()
	void AnimNotify_DarkBallShot();
	UFUNCTION()
	void AnimNotify_CreateTornado();
	UFUNCTION()
	void AnimNotify_FrostboltSpawn();
	UFUNCTION()
	void AnimNotify_FrostboltShot();
	UFUNCTION()
	void AnimNotify_Death();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	void MontageBlendingOut(UAnimMontage* Anim, bool Inter) override;

};
