#include "Character/MainCharacter.h"
#include "Global/GlobalAICharacter.h"
#include "Global/GlobalGameInstance.h"
#include "Global/Data/AnimaitionData.h"
#include "Components/CapsuleComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameUI/GameUIHUD.h"
#include "CollisionShape.h"
#include "DrawDebugHelpers.h"
#include "Materials/Material.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetActorTypeTag(TEXT("Player"));
	SetAttackTypeTag(TEXT("PlayerAttack"));

	Tags.Add(GetActorTypeTag());

	AimZoomTimelineLength = 1.f;

	BaseTurnRate = 30.f;
	BaseLookUpRate = 30.f;

	// ���¿� ����� ����
	IsLookAtTartget = false;
	MouseInput = false;
	MouseX = 0.f;
	MouseY = 0.f;
	MouseTimeCheck = 0.f;

	MainCameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	MainCameraSpringArmComponent->SetupAttachment(GetCapsuleComponent());
	MainCameraSpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight * 1.5f));
	MainCameraSpringArmComponent->bUsePawnControlRotation = true;
	MainCameraSpringArmComponent->TargetArmLength = 500.f;
	MainCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MainCameraComponent->SetupAttachment(MainCameraSpringArmComponent, USpringArmComponent::SocketName);
	MainCameraComponent->SetRelativeLocation(FVector(0.0f, 55.f, 0.f));
	
	FOVUpdateDelegate.BindUFunction(this, FName("AimZoomTimelineUpdate"));
	AimScreenUpdateDelegate.BindUFunction(this, FName("AimScreenTimelineUpdate"));
	ArmUpdateDelegate.BindUFunction(this, FName("ArmTimelineUpdate"));
	TimelineFinishDelegate.BindUFunction(this, FName("AimZoomOnFinish"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate.Yaw = 360.f;

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	FWeightedBlendable WeightedBlendable;
	WeightedBlendable.Weight = 0;
	WeightedBlendable.Object = nullptr;
	PostProcessComponent->Settings.WeightedBlendables.Array.Add(WeightedBlendable);
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->bUseControllerRotationYaw = false;

	// ü�� ����
	SetHP(100.f);
	SetMaxHP(GetHP());

	// �÷��̾�� ü�¹ٰ� ĳ���� ���� �� �ʿ䰡 ����
	SetMonsterHealthBarOnOff(false);

	// Ÿ�Ӷ��� ����
	if (nullptr != FOVCurveFloat && nullptr != AimScreenCurveFloat && nullptr != CameraSpringArmVector)
	{
		// Ŀ��� Ŀ�긦 ����� �Լ�
		ChangeViewFTimeline.AddInterpFloat(FOVCurveFloat, FOVUpdateDelegate);
		ChangeViewFTimeline.AddInterpFloat(AimScreenCurveFloat, AimScreenUpdateDelegate);
		ChangeViewFTimeline.AddInterpVector(CameraSpringArmVector, ArmUpdateDelegate);

		// ���� �� ȣ�� �� �Լ�
		ChangeViewFTimeline.SetTimelineFinishedFunc(TimelineFinishDelegate);

		// ���� ����
		ChangeViewFTimeline.SetTimelineLength(AimZoomTimelineLength);

		// ���� ����
		ChangeViewFTimeline.SetLooping(false);
	}

	// ����Ʈ ���μ��� ���� ��Ƽ���� ����
	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UMaterial* AimScreen = Instance->GetMaterialAsset(TEXT("AimScreen"));

	if (false == IsValid(AimScreen))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	DynMAimScreen = UMaterialInstanceDynamic::Create(AimScreen, this);

	PostProcessComponent->Settings.WeightedBlendables.Array[0].Object = DynMAimScreen;
}

void AMainCharacter::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	UWeaponComponent* CurWeaponComponent = GetWeaponComponent();

	if (false == IsValid(CurWeaponComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// ���� Ÿ�ٿ� ����
	LookAtTarget(_DeltaTime);

	EWeaponType CurWeponT = CurWeaponComponent->GetWeaponType();
	bool IsAim = CurWeaponComponent->GetIsAimOn();

	ChangeViewFTimeline.TickTimeline(_DeltaTime);

	APlayerController* HUDController = Cast<APlayerController>(GetController());
	AGameUIHUD* HUD = HUDController->GetHUD<AGameUIHUD>();

	if (nullptr == HUD || false == HUD->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	if (nullptr == DynMAimScreen || false == DynMAimScreen->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// ���ؽ� ī�޶� Ȯ��
	if (EWeaponType::Bow == CurWeponT && true == IsAim)
	{
		ChangeViewFTimeline.Play();

		CharTurnAim(_DeltaTime);

		HUD->GetMainWidget()->SetCrosshairOnOff(true);

		PostProcessComponent->Settings.WeightedBlendables.Array[0].Weight = 1;
	}
	else if (EWeaponType::Bow == CurWeponT && false == IsAim)
	{
		ChangeViewFTimeline.Reverse();

		HUD->GetMainWidget()->SetCrosshairOnOff(false);

		if (false == CurWeaponComponent->GetIsLockOn())
		{
			this->bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
	}
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_PlayerInputComponent);

	//check(_PlayerInputComponent);

	//UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("PlayerWheelUp", EKeys::MouseScrollUp));
	//UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("PlayerWheelDown", EKeys::MouseScrollDown));

	//UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Tab", EKeys::Tab));

	//UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("PlayerMouseLeft", EKeys::LeftMouseButton));
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMouseRight", EKeys::RightMouseButton, 1.f));

	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerTurn", EKeys::MouseX, 1.f));
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerLooKUp", EKeys::MouseY, -1.f));

	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::W, 1.f));
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::S, -1.f));
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::A, -1.f));
	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::D, 1.f));

	//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerRollorRun", EKeys::SpaceBar, 1.f));

	//UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("PlayerJump", EKeys::LeftShift));

	//UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("QuickSlot1", EKeys::One));
	//UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("QuickSlot2", EKeys::Two));
	//UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("QuickSlot3", EKeys::Three));

	//UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Parry", EKeys::E));

	//_PlayerInputComponent->BindAction("PlayerWheelUp", EInputEvent::IE_Pressed, this, &AMainCharacter::ZoomIn);
	//_PlayerInputComponent->BindAction("PlayerWheelDown", EInputEvent::IE_Pressed, this, &AMainCharacter::ZoomOut);

	//_PlayerInputComponent->BindAction("Tab", EInputEvent::IE_Pressed, this, &AMainCharacter::LockOnTarget);

	//_PlayerInputComponent->BindAction("PlayerMouseLeft", EInputEvent::IE_Pressed, this, &AMainCharacter::Attack);
	//_PlayerInputComponent->BindAxis("PlayerMouseRight", this, &AMainCharacter::AimorBlock);

	//_PlayerInputComponent->BindAxis("PlayerTurn", this, &AMainCharacter::AddControllerYawInput);
	//_PlayerInputComponent->BindAxis("PlayerLooKUp", this, &AMainCharacter::AddControllerPitchInput);

	//_PlayerInputComponent->BindAxis("PlayerMoveForward", this, &AMainCharacter::MoveForward);
	//_PlayerInputComponent->BindAxis("PlayerMoveRight", this, &AMainCharacter::MoveRight);

	//_PlayerInputComponent->BindAxis("PlayerRollorRun", this, &AMainCharacter::RollorRun);

	//_PlayerInputComponent->BindAction("PlayerJump", EInputEvent::IE_Pressed, this, &AMainCharacter::JumpAction);

	//_PlayerInputComponent->BindAction("QuickSlot1", EInputEvent::IE_Pressed, this, &AMainCharacter::ChangeUnArmed);
	//_PlayerInputComponent->BindAction("QuickSlot2", EInputEvent::IE_Pressed, this, &AMainCharacter::ChangeBow);
	//_PlayerInputComponent->BindAction("QuickSlot3", EInputEvent::IE_Pressed, this, &AMainCharacter::ChangeSwordAndSheiled);

	//_PlayerInputComponent->BindAction("Parry", EInputEvent::IE_Pressed, this, &AMainCharacter::Parry);
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	BpEventCallHPBar();

	UAudioComponent* CurAudio = GetAudioComponent();

	if (nullptr == CurAudio)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == Audio"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	FAnimaitionData* CurAnimaitionData = Instance->GetAnimaitionDatas("UnArmed");

	if (nullptr == CurAnimaitionData)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == CurAnimaitionData"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	APawn* EventInstigatorPawn = EventInstigator->GetPawn();

	if (false == IsValid(EventInstigatorPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	FVector HitDir = EventInstigatorPawn->GetActorLocation();
	HitDir.Z = 0;

	FVector CurPos = GetActorLocation();
	CurPos.Z = 0;

	FVector Dir = HitDir - CurPos;
	Dir.Normalize();

	UWeaponComponent* CurWeaponComponent = GetWeaponComponent();

	if (false == IsValid(CurWeaponComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	// �÷��̾� ĳ���ʹ� ���� �ʴ´�
	CurWeaponComponent->GotHit(Dir);

	// ��Ʈ ���� ť ����
	if (false == CurWeaponComponent->GetBlockSuccess() && false == CurWeaponComponent->GetParrySuccess())
	{
		// ��Ʈ ���� ť ����
		USoundCue* CurSoundCue = CurAnimaitionData->MapSoundCue.FindRef(CharacterAnimState::GotHit);

		if (nullptr == CurSoundCue)
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == CurSoundCue"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		CurAudio->SetSound(CurSoundCue);
	}
	
	// ���� ���
	if (nullptr == CurAudio->GetSound())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == CurAudio->GetSound()"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	CurAudio->Play();

	CurWeaponComponent->SetBlockSuccess(false);
	CurWeaponComponent->SetParrySuccess(false);

	return FinalDamage;
}

void AMainCharacter::ZoomIn()
{
	if (false == IsValid(MainCameraSpringArmComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	if (MainCameraSpringArmComponent->TargetArmLength > 150.f)
	{
		MainCameraSpringArmComponent->TargetArmLength *= 0.9;
	}
	else if (MainCameraSpringArmComponent->TargetArmLength < 150.f)
	{
		MainCameraSpringArmComponent->TargetArmLength = 150.f;
	}
}

void AMainCharacter::ZoomOut()
{
	if (false == IsValid(MainCameraSpringArmComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	if (MainCameraSpringArmComponent->TargetArmLength < 450.f)
	{
		MainCameraSpringArmComponent->TargetArmLength *= 1.1;
	}
	else if (MainCameraSpringArmComponent->TargetArmLength < 450.f)
	{
		MainCameraSpringArmComponent->TargetArmLength = 450.f;
	}
}

void AMainCharacter::Attack()
{
	if (false == IsValid(GetWeaponComponent()))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GetWeaponComponent()->AttackAction();
}

void AMainCharacter::MoveForward(float _Value)
{
	if (false == IsValid(GetWeaponComponent()))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GetWeaponComponent()->WAndSButtonAction(_Value);
}

void AMainCharacter::MoveRight(float _Value)
{
	if (false == IsValid(GetWeaponComponent()))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GetWeaponComponent()->DAndAButtonAction(_Value);
}

void AMainCharacter::RollorRun(float _Value)
{
	if (false == IsValid(GetWeaponComponent()))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GetWeaponComponent()->RollorRunAction(_Value);
}

void AMainCharacter::JumpAction()
{
	if (false == IsValid(GetWeaponComponent()))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GetWeaponComponent()->ShiftButtonAction();
}

void AMainCharacter::ChangeUnArmed()
{
	if (false == IsValid(GetWeaponComponent()))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GetWeaponComponent()->ChangeSetUnArmed();
}

void AMainCharacter::ChangeBow()
{
	if (false == IsValid(GetWeaponComponent()))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GetWeaponComponent()->ChangeSetBow();
}

void AMainCharacter::ChangeSwordAndSheiled()
{
	if (false == IsValid(GetWeaponComponent()))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GetWeaponComponent()->ChangeSetSwordAndSheiled();
}

void AMainCharacter::Parry()
{
	if (false == IsValid(GetWeaponComponent()))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GetWeaponComponent()->ParryAction();
}

void AMainCharacter::AimorBlock(float _Value)
{
	if (false == IsValid(GetWeaponComponent()))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GetWeaponComponent()->AimorBlockAtion(_Value);
}

void AMainCharacter::LockOnTarget()
{
	UWeaponComponent* CurWeaponComponent = GetWeaponComponent();

	if (false == IsValid(CurWeaponComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// �ø�/�÷�
	if (false == CurWeaponComponent->GetIsLockOn())
	{
		FVector Start = GetActorLocation(); // ���� ����
		Start.Z += BaseEyeHeight;
		FVector End = GetActorForwardVector() * LockOnTargetRange; // �� ����
		End = End.RotateAngleAxis(-50.f, FVector::UpVector); // ���� �������

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType; // ��Ʈ ������ ������Ʈ ����
		ObjectType.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
		ObjectType.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel5));

		TArray<AActor*> NotTargeting; // ������ ���͵�
		NotTargeting.Emplace(this);

		FHitResult HitRes; // ��Ʈ ��� ���� �޴� ����

		float TargetDistance = LockOnTargetRange; // ���� ����� ���Ϳ��� �Ÿ� ����
		AActor* HitNearActor = nullptr; // ���� ����� ���� ����

		for (int i = 0; i <= 100; i += 2) // �ݺ� �ϸ鼭 ������ �������� ����
		{
			FVector Direction = End.RotateAngleAxis(i, FVector::UpVector);
			FVector EndPoint = Start + Direction;

			bool IsHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, EndPoint, 30.f, ObjectType, false, NotTargeting, EDrawDebugTrace::ForDuration, HitRes, true);
			
			// ��Ʈ ��� ���� �������� Ȯ��
			bool IsMonster = false;
			if (true == IsHit)
			{
				AActor* HitResActor = HitRes.GetActor();
				IsMonster = HitResActor->ActorHasTag(TEXT("Monster"));
			}
			// ���Ͷ�� ����
			if (true == IsMonster && HitRes.Distance < TargetDistance)
			{
				TargetDistance = HitRes.Distance;
				HitNearActor = HitRes.GetActor();
			}
		}
		// ���� ����� ���� ����
		if (nullptr != HitNearActor)
		{
			CurWeaponComponent->SetIsLockOn(true);
			LockedOnTargetActor = Cast<AGlobalAICharacter>(HitNearActor);
			LockedOnTargetActor->SetAILockOnMarkOnOff(true);
			IsLookAtTartget = true;
		}
	}
	else if (true == CurWeaponComponent->GetIsLockOn())
	{
		LostLockedOnTargetActor();
	}
}

void AMainCharacter::LookAtTarget(float _DeltaTime)
{
	if (false == IsLookAtTartget)
	{
		// ���� ���� �ƴϴ�
		return;
	}

	// ���� �ð� �� MouseInput�� false�� �Ǹ� ȭ�� ����
	if (true == MouseInput)
	{
		MouseTimeCheck += _DeltaTime;
	}

	if (3.f <= MouseTimeCheck || CharacterAnimState::Attack == GetAnimState<CharacterAnimState>())
	{
		MouseInput = false;
		MouseTimeCheck = 0.f;
		MouseX = 0.f;
		MouseY = 0.f;
	}

	if (false == IsValid(LockedOnTargetActor))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UWeaponComponent* CurWeaponComponent = GetWeaponComponent();

	if (false == IsValid(CurWeaponComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	bool IsAim = CurWeaponComponent->GetIsAimOn();

	if (true == IsAim)
	{
		return;
	}
	if (true == CurWeaponComponent->GetIsCameraStares())
	{
		return;
	}

	APlayerController* PlayerCon = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (false == IsValid(PlayerCon))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	float MouseDeltaX = 0.f;
	float MouseDeltaY = 0.f;

	// ���콺�� �̵��� �޾ƿ�
	PlayerCon->GetInputMouseDelta(MouseDeltaX, MouseDeltaY);

	// ���밪���� ��ŭ �̵��ߴ��� Ȯ��
	MouseDeltaX = FMath::Abs(MouseDeltaX);
	MouseDeltaY = FMath::Abs(MouseDeltaY);
	

	// ���콺�� �������ٸ�
	if (0.f != MouseDeltaX || 0.f != MouseDeltaY)
	{
		MouseX += MouseDeltaX;
		MouseY += MouseDeltaY;

		MouseTimeCheck = 0.f;

		// ���콺�� ���� ��ġ �̻� �������ٸ�
		if ((8.f <= MouseX || 8.f <= MouseY) && false == MouseInput)
		{
			MouseInput = true;
			MouseX = 0.f;
			MouseY = 0.f;
		}

		UE_LOG(LogTemp, Warning, TEXT("Mouse Delta: %f, %f"), MouseDeltaX, MouseDeltaY);
		UE_LOG(LogTemp, Warning, TEXT("Mouse: %f, %f"), MouseX, MouseY);
	}
	// ���콺�� �������� �ʾҰ�, ���콺�� ���� ��ġ �̻� �������� �ʾҾ��ٸ�
	else if ((0.f == MouseDeltaX && 0.f == MouseDeltaY) && false == MouseInput)
	{
		MouseX = 0.f;
		MouseY = 0.f;

		FVector LockOnLocation = LockedOnTargetActor->GetActorLocation();

		FVector CurPos = GetActorLocation();
		CurPos.Z = 0;
		LockOnLocation.Z = 0;

		FVector Dir = LockOnLocation - CurPos;
		Dir.Normalize();

		FVector OtherForward = GetActorForwardVector();
		OtherForward.Normalize();

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = OtherForward.Rotation().Yaw;

		if (10.f >= FMath::Abs(Angle0 - Angle1))
		{
			this->bUseControllerRotationYaw = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CurPos, LockOnLocation);
		FRotator InterpRotation = UKismetMathLibrary::RInterpTo(GetController()->GetControlRotation(), LookAtRotation, _DeltaTime, 5.f);

		FRotator LookAtActor = FRotator(InterpRotation.Pitch, InterpRotation.Yaw, GetController()->GetControlRotation().Roll);

		GetController()->SetControlRotation(LookAtActor);
		SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), LookAtRotation, _DeltaTime, 5.f));
	}
}

void AMainCharacter::CharTurnAim(float _DeltaTime)
{
	FRotator CurRot = GetTransform().Rotator();
	FRotator ControlRot = GetControlRotation();

	CurRot.Pitch = 0;
	ControlRot.Pitch = 0;

	FRotator Dir = ControlRot - CurRot;
	Dir.Normalize();

	CurRot.Normalize();
	ControlRot.Normalize();

	float Angle0 = CurRot.Yaw;
	float Angle1 = ControlRot.Yaw;

	if (5.f <= FMath::Abs(Angle0 - Angle1))
	{
		FRotator Rot = Dir * 7 * _DeltaTime;
		AddActorWorldRotation(Rot);
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		this->bUseControllerRotationYaw = true;
	}
}

void AMainCharacter::AimZoomTimelineUpdate(float _Value)
{
	// FOV�� ���� �۾�����
	MainCameraComponent->FieldOfView = _Value;
}

void AMainCharacter::AimScreenTimelineUpdate(float _Value)
{
	// ���ؽ� ȭ�� �׵θ��� ��ο�����
	DynMAimScreen->SetScalarParameterValue(TEXT("Alpha"), _Value);
}

void AMainCharacter::ArmTimelineUpdate(FVector _Value)
{
	// ���ؽ� ī�޶� ���̰� ��¦ ��������
	MainCameraSpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight * _Value.Z));
}

void AMainCharacter::AimZoomOnFinish()
{

}

FVector AMainCharacter::CameraLineTrace()
{
	FVector TargetVector = FVector::ZeroVector;

	if (false == IsValid(MainCameraSpringArmComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return TargetVector;
	}

	UWeaponComponent* CurWeaponComponent = GetWeaponComponent();

	if (false == IsValid(CurWeaponComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return TargetVector;
	}

	FHitResult HitRes;
	FVector CameraForwardVector = MainCameraComponent->GetForwardVector();

	// ���� ������ ī�޶�κ��� �������� ��ŭ ������ ����.
	FVector StartLocationVector = MainCameraComponent->GetComponentTransform().GetTranslation();
	StartLocationVector += CameraForwardVector * MainCameraSpringArmComponent->TargetArmLength;

	// �� ������ ī�޶�κ��� X������ �����̴�.
	FVector End = StartLocationVector + (CameraForwardVector * 2000.f);

	// ���� Ʈ���̽� ä��
	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;

	FCollisionQueryParams QueryParams;

	// ���� Ʈ���̽��� �����ϴ� ���� ĳ���͸� �����Ѵ�.
	QueryParams.AddIgnoredActor(this);

	// ���� Ʈ���̽� ����
	GetWorld()->LineTraceSingleByChannel(HitRes, StartLocationVector, End, Channel, QueryParams);

	DrawDebugLine(GetWorld(), StartLocationVector, End, FColor::Red);

	if (nullptr == HitRes.GetActor())
	{
		// �� ���� ����
		TargetVector = End;
	}
	else if (nullptr != HitRes.GetActor())
	{
		// ���� Ʈ���̽��� �浹�� ����
		TargetVector = HitRes.ImpactPoint;
	}

	// ȭ���� ���ư� ������ ���Ѵ�
	FVector Joint = CurWeaponComponent->GetBowJointLocation();
	FVector ReturnVector = TargetVector - Joint;

	// Normalize ������Ѵ�.
	ReturnVector.Normalize();

	return ReturnVector;
}

void AMainCharacter::LostLockedOnTargetActor()
{
	UWeaponComponent* CurWeaponComponent = GetWeaponComponent();

	if (false == IsValid(CurWeaponComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	bool LockOnCheck = CurWeaponComponent->GetIsLockOn();

	if (true == IsValid(LockedOnTargetActor))
	{
		LockedOnTargetActor->SetAILockOnMarkOnOff(false);
		LockedOnTargetActor = nullptr;
	}
	else if (true == LockOnCheck && false == IsValid(LockedOnTargetActor))
	{
		// ���� ���ε� ���� Ÿ���� IsValid() ���� false�̹Ƿ� Error
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> true == LockOnCheck && false == IsValid(LockedOnTargetActor)"), __FUNCTION__, __LINE__);
	}

	IsLookAtTartget = false;
	MouseInput = false;
	MouseX = 0.f;
	MouseY = 0.f;
	MouseTimeCheck = 0.f;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CurWeaponComponent->SetIsLockOn(false);
	CurWeaponComponent->SetIsCameraStares(false);
}
