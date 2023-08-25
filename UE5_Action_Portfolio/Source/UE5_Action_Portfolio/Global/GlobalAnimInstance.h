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
	virtual void MontageBlendingOut(UAnimMontage* Anim, bool Inter);

	template<typename EnumType>
	EnumType GetAnimState()
	{
		return static_cast<EnumType>(Animstate);
	}
	template<typename EnumType>
	void SetAnimState(EnumType _AnimState)
	{
		Animstate = static_cast<int>(_AnimState);

	}
	void SetAnimState(int _AnimState);
	template<typename EnumType>
	UAnimMontage* GetAnimMontage(EnumType _Index)
	{
		return GetAnimMontage(static_cast<int>(_Index));
	}
	UAnimMontage* GetAnimMontage(int _Index);
	template<typename EnumType>
	void SetAllAnimation(const TMap<EnumType, UAnimMontage*>& _MapAnimation)
	{
		for (TPair<EnumType, UAnimMontage*> Pair : _MapAnimation)
		{
			AllAnimations.Add(static_cast<int>(Pair.Key), Pair.Value);
		}
	}

protected:
	void NativeInitializeAnimation() override;
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int Animstate = 0;
	UPROPERTY()
		TMap<int, UAnimMontage*> AllAnimations;
};