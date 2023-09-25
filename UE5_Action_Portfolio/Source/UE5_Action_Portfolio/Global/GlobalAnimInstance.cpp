#include "Global/GlobalAnimInstance.h"
#include "Global/GlobalCharacter.h"

void UGlobalAnimInstance::MontageBlendingOut(UAnimMontage* Anim, bool Inter)
{

}

void UGlobalAnimInstance::AnimNotify_Death()
{
	AGlobalCharacter* Character = Cast<AGlobalCharacter>(GetOwningActor());

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	Character->Destroy();
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
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> 0 == AllAnimations.Num()"), __FUNCTION__, __LINE__);
		return;
	}

	AGlobalCharacter* Character = Cast<AGlobalCharacter>(GetOwningActor());

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UAnimMontage* Montage = AllAnimations[Character->GetAnimState()];

	if (false == IsValid(Montage))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
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