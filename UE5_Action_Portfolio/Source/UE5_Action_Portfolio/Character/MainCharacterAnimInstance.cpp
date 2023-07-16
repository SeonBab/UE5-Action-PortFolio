#include "Character/MainCharacterAnimInstance.h"
#include "MainCharacter.h"
#include "Global/GlobalGameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Global/WeaponData.h"
#include "Weapon/WeaponAction.h"

void UMainCharacterAnimInstance::MontageEnd(UAnimMontage* Anim, bool Inter)
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	if (AllAnimations[MainCharacterAnimState::WalkJump] == Anim || AllAnimations[MainCharacterAnimState::RunJump] == Anim || AllAnimations[MainCharacterAnimState::Roll] == Anim)
	{
		Animstate = MainCharacterAnimState::Idle;
		character->CurWeapon->SetAnimState(Animstate);
		Montage_Play(AllAnimations[MainCharacterAnimState::Idle], 1.0f);
	}
}

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	//CurCharacter = Cast<AMainCharacter>(GetOwningActor());

	//if (nullptr == CurCharacter)
	//{
	//	return;
	//}

	//CurCharacterMovementComponent = CurCharacter->GetMovementComponent();

	//if (nullptr == CurCharacterMovementComponent)
	//{
	//	return;
	//}

	//CurCharSpeed = 0;
}

void UMainCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageBlendingOut.AddDynamic(this, &UMainCharacterAnimInstance::MontageEnd);

	UGlobalGameInstance* Ins = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Ins)
	{
		return;
	}

	FWeaponData* FindWeaponData = Ins->GetWeaponData(TEXT("UnArmed"));

	if (nullptr == FindWeaponData)
	{
		return;
	}

	AllAnimations = FindWeaponData->AllAnimations;
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (0 == AllAnimations.Num())
	{
		return;
	}

	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	Animstate = *character->AnimState;

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
