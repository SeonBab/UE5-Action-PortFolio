#include "Global/GlobalAnimInstance.h"

void UGlobalAnimInstance::MontageBlendingOut(UAnimMontage* Anim, bool Inter)
{

}

void UGlobalAnimInstance::SetAnimState(int _AnimState)
{
	Animstate = _AnimState;
}

UAnimMontage* UGlobalAnimInstance::GetAnimMontage(int _Index)
{
	if (false == AllAnimations.Contains(_Index))
	{
		return nullptr;
	}

	return AllAnimations[_Index];
}

void UGlobalAnimInstance::NativeInitializeAnimation()
{
}

void UGlobalAnimInstance::NativeBeginPlay()
{
}

void UGlobalAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
}
