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
	void AnimNotify_Craete_Dark_Ball();
	UFUNCTION()
	void AnimNotify_Dark_Ball_Shot();


protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	void MontageBlendingOut(UAnimMontage* Anim, bool Inter) override;

};
