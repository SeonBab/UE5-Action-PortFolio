#include "MainPlayerChar.h"
#include "Global/GlobalGameInstance.h"
#include "Camera/CameraComponent.h"
#include <Global/Enums.h>
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Global/WeaponData.h>
#include "PlayerAnimInstance.h"

AMainPlayerChar::AMainPlayerChar()
{
	PrimaryActorTick.bCanEverTick = true;

	TPSCameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSCameraSpringArm"));
	TPSCameraSpringArmComponent->SetupAttachment(GetCapsuleComponent());
	TPSCameraSpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	TPSCameraSpringArmComponent->bUsePawnControlRotation = true;
	TPSCameraSpringArmComponent->TargetArmLength = 450.f;

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCamera"));
	TPSCameraComponent->SetupAttachment(TPSCameraSpringArmComponent, USpringArmComponent::SocketName);

	BaseTurnRate = 30.f;
	BaseLookUpRate = 30.f;

}

void AMainPlayerChar::BeginPlay()
{
	Super::BeginPlay();
	
	ChangeUnarm();
	AniState = AnimPlayerState::Idle;
}

void AMainPlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainPlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("PlayerWheelUp", EKeys::MouseScrollUp));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("PlayerWheelDown", EKeys::MouseScrollDown));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerTurn", EKeys::MouseX, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerLooKUp", EKeys::MouseY, 1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::W, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::S, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::A, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::D, 1.f));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("PlayerJump", EKeys::LeftShift));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ChangeUnarm", EKeys::O));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ChangeSword", EKeys::P));

	PlayerInputComponent->BindAction("PlayerWheelUp", EInputEvent::IE_Pressed, this, &AMainPlayerChar::ZoomIn);
	PlayerInputComponent->BindAction("PlayerWheelDown", EInputEvent::IE_Pressed, this, &AMainPlayerChar::ZoomOut);

	PlayerInputComponent->BindAxis("PlayerTurn", this, &AMainPlayerChar::AddControllerYawInput);
	PlayerInputComponent->BindAxis("PlayerLooKUp", this, &AMainPlayerChar::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("PlayerMoveForward", this, &AMainPlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("PlayerMoveRight", this, &AMainPlayerChar::MoveRight);

	PlayerInputComponent->BindAction("ChangeUnarm", EInputEvent::IE_Pressed, this, &AMainPlayerChar::ChangeUnarm);
	PlayerInputComponent->BindAction("ChangeSword", EInputEvent::IE_Pressed, this, &AMainPlayerChar::ChangeSword);


	PlayerInputComponent->BindAction("PlayerJump", EInputEvent::IE_Pressed, this, &AMainPlayerChar::JumpAction);
}

void AMainPlayerChar::ZoomIn()
{
	if (TPSCameraSpringArmComponent->TargetArmLength > 50.f)
	{
		TPSCameraSpringArmComponent->TargetArmLength *= 0.9;
	}
}

void AMainPlayerChar::ZoomOut()
{
	if (TPSCameraSpringArmComponent->TargetArmLength < 450.f)
	{
		TPSCameraSpringArmComponent->TargetArmLength *= 1.1;
	}
}

void AMainPlayerChar::MoveForward(float Value)
{
	if (nullptr != Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);

		//AniState = Value > 0.f ? AniPlayerState::ForwardMove : AniPlayerState::BackwardMove;
	}
	//else
	//{
	//	if (AniState == AniPlayerState::ForwardMove || AniState == AniPlayerState::BackwardMove)
	//	{
	//		AniState = AniPlayerState::Idle;
	//	}
	//}
}

void AMainPlayerChar::MoveRight(float Value)
{
	if (nullptr != Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);

		//AniState = Value > 0.f ? AniPlayerState::RightMove : AniPlayerState::LeftMove;
	}
	//else
	//{
	//	if (AniState == AniPlayerState::LeftMove || AniState == AniPlayerState::RightMove)
	//	{
	//		AniState = AniPlayerState::Idle;
	//	}
	//}
}

void AMainPlayerChar::JumpAction()
{
	Jump();
	//AniState = AniPlayerState::Jump;
}

void AMainPlayerChar::ChangeWeapon(FName _Weapon)
{
	UPlayerAnimInstance* Ptr = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (nullptr == Ptr)
	{
		return;
	}

	UGlobalGameInstance* Instacne = GetWorld()->GetGameInstance<UGlobalGameInstance>();


	const struct FWeaponData* FindWeaponData = Instacne->GetWeaponData(_Weapon);

	if (nullptr == FindWeaponData)
	{
		return;
	}

	CurWeapon = FindWeaponData;

	Ptr->AllAnimations = CurWeapon->AllAnimations;
	
}

void AMainPlayerChar::ChangeUnarm() 
{
	ChangeWeapon(TEXT("Unarm"));
}

void AMainPlayerChar::ChangeSword() 
{
	ChangeWeapon(TEXT("Sword"));
}