#include "AI/Monster/MonsterAnimInstance.h"
#include "AI/Monster/MonsterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Global/Enums.h"

void UMonsterAnimInstance::AnimNotify_MeleeStart()
{
	AMonsterCharacter* CurMonster = Cast<AMonsterCharacter>(GetOwningActor());

	if (false == IsValid(CurMonster))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UCapsuleComponent* CapsuleComponent = CurMonster->GetMeleeCapsuleComponent();

	if (false == IsValid(CapsuleComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// 콜리전 설정
	FName AttackType = CurMonster->GetAttackTypeTag();

	CapsuleComponent->SetCollisionProfileName(AttackType);
}

void UMonsterAnimInstance::AnimNotify_MeleeEnd()
{
	AMonsterCharacter* CurMonster = Cast<AMonsterCharacter>(GetOwningActor());

	if (false == IsValid(CurMonster))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UCapsuleComponent* CapsuleComponent = CurMonster->GetMeleeCapsuleComponent();

	if (false == IsValid(CapsuleComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// 콜리전 설정
	CapsuleComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

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
	AGlobalAICharacter* Character = Cast<AGlobalAICharacter>(GetOwningActor());

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
