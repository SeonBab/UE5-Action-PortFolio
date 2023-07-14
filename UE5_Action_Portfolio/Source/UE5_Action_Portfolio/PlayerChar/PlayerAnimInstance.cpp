// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "MainPlayerChar.h"
#include "Global/GlobalGameInstance.h"
#include "Components/SkeletalMeshComponent.h"


void UPlayerAnimInstance::MontageEnd(UAnimMontage* Anim, bool Inter)
{
	TSubclassOf<UPlayerAnimInstance> inst = UPlayerAnimInstance::StaticClass();

	AMainPlayerChar* character = Cast<AMainPlayerChar>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	//리턴하고 터진다???
	if (false == AllAnimations.Contains(AnimPlayerState::WalkJump) || false == AllAnimations.Contains(AnimPlayerState::RunJump))
	{
		return;
	}

	// 체크 안해도 되나???
	if (AllAnimations[AnimPlayerState::WalkJump] == Anim || AllAnimations[AnimPlayerState::RunJump] == Anim)
	{
		Animstate = AnimPlayerState::Idle;
		character->AniState = Animstate;
		Montage_Play(AllAnimations[AnimPlayerState::Idle], 1.0f);
	}
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageBlendingOut.AddDynamic(this, &UPlayerAnimInstance::MontageEnd);

	UGlobalGameInstance* Ins = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Ins)
	{
		return;
	}

	AllAnimations = Ins->GetAllAnimations(TEXT("UnArmed"));
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (0 == AllAnimations.Num())
	{
		return;
	}

	AMainPlayerChar* character = Cast<AMainPlayerChar>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	Animstate = character->AniState;

	if (false == AllAnimations.Contains(Animstate))
	{
		return;
	}

	UAnimMontage* montage = AllAnimations[Animstate];

	if (nullptr == montage)
	{
		return;
	}

	if (false == Montage_IsPlaying(montage))
	{
		Montage_Play(montage, 1.0f);
	}
}
