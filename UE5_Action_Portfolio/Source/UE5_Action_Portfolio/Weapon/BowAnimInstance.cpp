#include "Weapon/BowAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

void UBowAnimInstance::InitializeAnimation()
{
}

void UBowAnimInstance::NativeBeginPlay()
{
}

void UBowAnimInstance::NativeUpdateAnimation(float _DeltaTime)
{
	
}

const bool UBowAnimInstance::GetBowChordCheck()
{
	return BowChordCheck;
}

void UBowAnimInstance::SetBowChordCheck(bool _Value)
{
	BowChordCheck = _Value;
}


void UBowAnimInstance::SetHandTransform(const FVector _HandTransform)
{
	HandTransform = _HandTransform;
}
