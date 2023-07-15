#include "Character/MainCharacterAnimInstance.h"

//#include "Components/SkeletalMeshComponent.h"


void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	CurCharacter = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == CurCharacter)
	{
		return;
	}

	CurCharacterMovementComponent = CurCharacter->GetMovementComponent();

	if (nullptr == CurCharacterMovementComponent)
	{
		return;
	}

	CurCharSpeed = 0;
}

void UMainCharacterAnimInstance::NativeBeginPlay()
{
	
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{

}
