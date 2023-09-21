#include "Global/GlobalAnimInstance.h"
#include "Global/GlobalCharacter.h"

void UGlobalAnimInstance::MontageBlendingOut(UAnimMontage* Anim, bool Inter)
{

}

UAnimMontage* UGlobalAnimInstance::GetAnimMontage(int _Index)
{
	if (false == AllAnimations.Contains(_Index))
	{
		return nullptr;
	}

	return AllAnimations[_Index];
}

TMap<int, UAnimMontage*> UGlobalAnimInstance::GetAllAnimations()
{
	return AllAnimations;
}

void UGlobalAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UGlobalAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OnMontageEnded.AddDynamic(this, &UGlobalAnimInstance::MontageEnd);
}

void UGlobalAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (0 == AllAnimations.Num())
	{
		return;
	}

	AGlobalCharacter* Character = Cast<AGlobalCharacter>(GetOwningActor());

	if (false == IsValid(Character))
	{
		return;
	}

	UAnimMontage* Montage = AllAnimations[Character->GetAnimState()];

	if (false == IsValid(Montage))
	{
		return;
	}

	if (false == Montage_IsPlaying(Montage))
	{
		Montage_Play(Montage, 1.f);
	}
}

void UGlobalAnimInstance::MontageEnd(UAnimMontage* Anim, bool _Inter)
{

}