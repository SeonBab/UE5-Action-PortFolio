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

void UBowAnimInstance::SetHandTransform(const FTransform _HandTransform)
{
	FVector Pos = _HandTransform.GetLocation();

	HandTransform = Pos;
}
