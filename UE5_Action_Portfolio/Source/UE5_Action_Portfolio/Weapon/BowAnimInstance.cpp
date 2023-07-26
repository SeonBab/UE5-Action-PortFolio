#include "Weapon/BowAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

void UBowAnimInstance::InitializeAnimation()
{
	//CurSkeletalMeshComponenet = GetOwningComponent();
}

void UBowAnimInstance::NativeBeginPlay()
{
}

void UBowAnimInstance::NativeUpdateAnimation()
{
	
}

void UBowAnimInstance::SetHandTransform(const FVector _HandTransform)
{
	HandTransform = _HandTransform;
}
