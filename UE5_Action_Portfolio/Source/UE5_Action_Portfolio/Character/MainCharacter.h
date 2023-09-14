#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalCharacter.h"
#include "Global/Enums.h"
#include "Global/FootIKComponent.h"
#include "Components/TimelineComponent.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWeaponComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API AMainCharacter : public AGlobalCharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	// 조작
	void ZoomIn();
	void ZoomOut();
	void Attack();
	void MoveForward(float _Value);
	void MoveRight(float _Value);
	void RollorRun(float _Value);
	void JumpAction();
	void ChangeUnArmed();
	void ChangeBow();
	void ChangeSwordAndSheiled();
	void Parry();
	void AimorBlock(float _Value);
	void LockOnTarget();
	void LookAtTarget(float _DeltaTime);
	void CharTurnAim(float _DeltaTime);
	FVector CameraLineTrace();

	UFUNCTION(BlueprintCallable)
	void LostLockedOnTargetActor();
	UFUNCTION(BlueprintCallable)
	UWeaponComponent* GetWeaponComponent();

	UFUNCTION(BlueprintImplementableEvent)
	void BpEventCallHPBar();
	UFUNCTION(BlueprintImplementableEvent)
	void BpEventChangeWpSlot();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* _PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UFUNCTION()
	void AimZoomTimelineUpdate(float _Value);
	UFUNCTION()
	void ArmTimelineUpdate(FVector _Value);
	UFUNCTION()
	void AimZoomOnFinish();

public:
	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* MainCameraSpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* MainCameraComponent;
	float BaseTurnRate;
	float BaseLookUpRate;
	float LockOnTargetRange = 2500.f;
	AActor* LockedOnTargetActor = nullptr;

private:
	bool IsLookAtTartget = false;
	bool MouseInput = false;
	float MouseX = 0.f;
	float MouseY = 0.f;
	float MouseTimeCheck = 0.f;


	FTimeline ChangeViewFTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* FOVCurveFloat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveVector* CameraSpringArmVector;
	FOnTimelineFloat FOVUpdateDelegate;
	FOnTimelineVector ArmUpdateDelegate;
	FOnTimelineEvent TimelineFinishDelegate;
	float AimZoomTimelineLength = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UFootIKComponent* FootIKComponent;
	FFootIKOffset FootIKOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWeaponComponent* WeaponComponent;

	protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator FootRotatorLeft;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator FootRotatorRight;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float HipOffset;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FootOffsetLeft;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FootOffsetRight;
	float CurCapsuleSize;
};
