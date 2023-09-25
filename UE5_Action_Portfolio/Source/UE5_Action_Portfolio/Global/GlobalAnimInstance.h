#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GlobalAnimInstance.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UGlobalAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void AnimNotify_Death();

	UAnimMontage* GetAnimMontage(int _Index);
	template<typename EnumType>
	UAnimMontage* GetAnimMontage(EnumType _Index)
	{
		return GetAnimMontage(static_cast<int>(_Index));
	}
	template<typename EnumType>
	void SetAllAnimation(const TMap<EnumType, UAnimMontage*>& _MapAnimation)
	{
		for (TPair<EnumType, UAnimMontage*> Pair : _MapAnimation)
		{
			AllAnimations.Add(static_cast<int>(Pair.Key), Pair.Value);
		}
	}
	TMap<int, UAnimMontage*> GetAllAnimations();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	UFUNCTION()
	virtual void MontageEnd(UAnimMontage* Anim, bool _Inter);
	UFUNCTION()
	virtual void MontageBlendingOut(UAnimMontage* Anim, bool Inter);
public:
	float AnimSpeed = 1.f;

	UPROPERTY()
	TMap<int, UAnimMontage*> AllAnimations;

};