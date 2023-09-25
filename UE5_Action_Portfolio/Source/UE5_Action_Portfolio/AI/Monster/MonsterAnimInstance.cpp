#include "AI/Monster/MonsterAnimInstance.h"
#include "AI/Monster/MonsterCharacter.h"
#include "Global/Enums.h"

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageBlendingOut.AddDynamic(this, &UMonsterAnimInstance::MontageBlendingOut);
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
}

void UMonsterAnimInstance::MontageBlendingOut(UAnimMontage* Anim, bool Inter)
{
	AGlobalCharacter* Character = Cast<AGlobalCharacter>(GetOwningActor());

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	AIAnimState AnimState = static_cast<AIAnimState>(Character->GetAnimState());

	if (AIAnimState::Death == AnimState)
	{
		return;
	}

	if (Anim == GetAnimMontage(BossAnimState::GotHit))
	{
		Character->SetAnimState(BossAnimState::Idle);
		Montage_Play(AllAnimations[Character->GetAnimState()], 1.f);
	}
}
