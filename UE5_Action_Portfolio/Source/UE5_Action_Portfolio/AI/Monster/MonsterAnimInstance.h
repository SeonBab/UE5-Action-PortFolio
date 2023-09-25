#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalAnimInstance.h"
#include "MonsterAnimInstance.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UMonsterAnimInstance : public UGlobalAnimInstance
{
	GENERATED_BODY()
public:

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	void MontageBlendingOut(UAnimMontage* Anim, bool Inter) override;
};
