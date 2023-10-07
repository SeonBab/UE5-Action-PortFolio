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

	// 락온에 사용할 변수
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

	// 체력 설정
	SetHP(100.f);
	SetMaxHP(GetHP());

	// 플레이어는 체력바가 캐릭터 위에 뜰 필요가 없다
	SetMonsterHealthBarOnOff(false);

	// 타임라인 설정
	if (nullptr != FOVCurveFloat && nullptr != AimScreenCurveFloat && nullptr != CameraSpringArmVector)
	{
		// 커브와 커브를 사용할 함수
		ChangeViewFTimeline.AddInterpFloat(FOVCurveFloat, FOVUpdateDelegate);
		ChangeViewFTimeline.AddInterpFloat(AimScreenCurveFloat, AimScreenUpdateDelegate);
		ChangeViewFTimeline.AddInterpVector(CameraSpringArmVector, ArmUpdateDelegate);

		// 끝날 때 호출 할 함수
		ChangeViewFTimeline.SetTimelineFinishedFunc(TimelineFinishDelegate);

		// 길이 설정
		ChangeViewFTimeline.SetTimelineLength(AimZoomTimelineLength);

		// 루프 끄기
		ChangeViewFTimeline.SetLooping(false);
	}

	// 포스트 프로세스 조준 머티리얼 설정
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

	// 락온 타겟에 고정
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

	// 조준시 카메라 확대
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

	// 플레이어 캐릭터는 죽지 않는다
	CurWeaponComponent->GotHit(Dir);

	// 히트 사운드 큐 설정
	if (false == CurWeaponComponent->GetBlockSuccess() && false == CurWeaponComponent->GetParrySuccess())
	{
		// 히트 사운드 큐 설정
		USoundCue* CurSoundCue = CurAnimaitionData->MapSoundCue.FindRef(CharacterAnimState::GotHit);

		if (nullptr == CurSoundCue)
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == CurSoundCue"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		CurAudio->SetSound(CurSoundCue);
	}
	
	// 사운드 재생
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

	// 플립/플롭
	if (false == CurWeaponComponent->GetIsLockOn())
	{
		FVector Start = GetActorLocation(); // 시작 지점
		Start.Z += BaseEyeHeight;
		FVector End = GetActorForwardVector() * LockOnTargetRange; // 끝 지점
		End = End.RotateAngleAxis(-50.f, FVector::UpVector); // 왼쪽 방향부터

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType; // 히트 가능한 오브젝트 유형
		ObjectType.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
		ObjectType.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel5));

		TArray<AActor*> NotTargeting; // 무시할 액터들
		NotTargeting.Emplace(this);

		FHitResult HitRes; // 히트 결과 값을 받는 변수

		float TargetDistance = LockOnTargetRange; // 가장 가까운 액터와의 거리 저장
		AActor* HitNearActor = nullptr; // 가장 가까운 액터 저장

		for (int i = 0; i <= 100; i += 2) // 반복 하면서 오른쪽 방향으로 돌림
		{
			FVector Direction = End.RotateAngleAxis(i, FVector::UpVector);
			FVector EndPoint = Start + Direction;

			bool IsHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, EndPoint, 30.f, ObjectType, false, NotTargeting, EDrawDebugTrace::ForDuration, HitRes, true);
			
			// 히트 결과 값이 몬스터인지 확인
			bool IsMonster = false;
			if (true == IsHit)
			{
				AActor* HitResActor = HitRes.GetActor();
				IsMonster = HitResActor->ActorHasTag(TEXT("Monster"));
			}
			// 몬스터라면 저장
			if (true == IsMonster && HitRes.Distance < TargetDistance)
			{
				TargetDistance = HitRes.Distance;
				HitNearActor = HitRes.GetActor();
			}
		}
		// 가장 가까운 몬스터 락온
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
		// 락온 중이 아니다
		return;
	}

	// 일정 시간 후 MouseInput는 false가 되며 화면 고정
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

	// 마우스의 이동을 받아옴
	PlayerCon->GetInputMouseDelta(MouseDeltaX, MouseDeltaY);

	// 절대값으로 얼만큼 이동했는지 확인
	MouseDeltaX = FMath::Abs(MouseDeltaX);
	MouseDeltaY = FMath::Abs(MouseDeltaY);
	

	// 마우스가 움직였다면
	if (0.f != MouseDeltaX || 0.f != MouseDeltaY)
	{
		MouseX += MouseDeltaX;
		MouseY += MouseDeltaY;

		MouseTimeCheck = 0.f;

		// 마우스가 일정 수치 이상 움직였다면
		if ((8.f <= MouseX || 8.f <= MouseY) && false == MouseInput)
		{
			MouseInput = true;
			MouseX = 0.f;
			MouseY = 0.f;
		}

		UE_LOG(LogTemp, Warning, TEXT("Mouse Delta: %f, %f"), MouseDeltaX, MouseDeltaY);
		UE_LOG(LogTemp, Warning, TEXT("Mouse: %f, %f"), MouseX, MouseY);
	}
	// 마우스가 움직이지 않았고, 마우스가 일정 수치 이상 움직이지 않았었다면
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
	// FOV의 값이 작아진다
	MainCameraComponent->FieldOfView = _Value;
}

void AMainCharacter::AimScreenTimelineUpdate(float _Value)
{
	// 조준시 화면 테두리가 어두워진다
	DynMAimScreen->SetScalarParameterValue(TEXT("Alpha"), _Value);
}

void AMainCharacter::ArmTimelineUpdate(FVector _Value)
{
	// 조준시 카메라 높이가 살짝 낮아진다
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

	// 시작 지점은 카메라로부터 스프링암 만큼 앞으로 간다.
	FVector StartLocationVector = MainCameraComponent->GetComponentTransform().GetTranslation();
	StartLocationVector += CameraForwardVector * MainCameraSpringArmComponent->TargetArmLength;

	// 끝 지점은 카메라로부터 X값으로 직선이다.
	FVector End = StartLocationVector + (CameraForwardVector * 2000.f);

	// 라인 트레이스 채널
	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;

	FCollisionQueryParams QueryParams;

	// 라인 트레이스를 실행하는 메인 캐릭터를 무시한다.
	QueryParams.AddIgnoredActor(this);

	// 라인 트레이스 실행
	GetWorld()->LineTraceSingleByChannel(HitRes, StartLocationVector, End, Channel, QueryParams);

	DrawDebugLine(GetWorld(), StartLocationVector, End, FColor::Red);

	if (nullptr == HitRes.GetActor())
	{
		// 끝 지점 벡터
		TargetVector = End;
	}
	else if (nullptr != HitRes.GetActor())
	{
		// 라인 트레이스가 충돌한 벡터
		TargetVector = HitRes.ImpactPoint;
	}

	// 화살이 날아갈 방향을 구한다
	FVector Joint = CurWeaponComponent->GetBowJointLocation();
	FVector ReturnVector = TargetVector - Joint;

	// Normalize 해줘야한다.
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
		// 락온 중인데 락온 타겟의 IsValid() 값이 false이므로 Error
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
