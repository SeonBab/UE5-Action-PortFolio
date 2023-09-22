#pragma once

#include "CoreMinimal.h"
#include "AI/AIWeaponCharacter.h"
#include "Global/Enums.h"
#include "Components/TimelineComponent.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPostProcessComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API AMainCharacter : public AAIWeaponCharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	// 조작
	UFUNCTION(BlueprintCallable)
	void ZoomIn();
	UFUNCTION(BlueprintCallable)
	void ZoomOut();
	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	void MoveForward(float _Value);
	UFUNCTION(BlueprintCallable)
	void MoveRight(float _Value);
	UFUNCTION(BlueprintCallable)
	void RollorRun(float _Value);
	UFUNCTION(BlueprintCallable)
	void JumpAction();
	UFUNCTION(BlueprintCallable)
	void ChangeUnArmed();
	UFUNCTION(BlueprintCallable)
	void ChangeBow();
	UFUNCTION(BlueprintCallable)
	void ChangeSwordAndSheiled();
	UFUNCTION(BlueprintCallable)
	void Parry();
	UFUNCTION(BlueprintCallable)
	void AimorBlock(float _Value);
	UFUNCTION(BlueprintCallable)
	void LockOnTarget();

	// 락온
	void LookAtTarget(float _DeltaTime);
	void CharTurnAim(float _DeltaTime);
	FVector CameraLineTrace();

	UFUNCTION(BlueprintCallable)
	void LostLockedOnTargetActor();

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
	void AimScreenTimelineUpdate(float _Value);
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
	AGlobalAICharacter* LockedOnTargetActor = nullptr;

private:
	// 락온에 사용하는 변수
	bool IsLookAtTartget;
	bool MouseInput;
	float MouseX;
	float MouseY;
	float MouseTimeCheck;


	// 활 조준할 때 사용하는 커브
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* FOVCurveFloat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* AimScreenCurveFloat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveVector* CameraSpringArmVector;
	// 타임라인의 길이를 설정할 변수
	FTimeline ChangeViewFTimeline;
	float AimZoomTimelineLength;
	// 델리게이트를 사용해 커브와 함수 바인드
	FOnTimelineFloat FOVUpdateDelegate;
	FOnTimelineFloat AimScreenUpdateDelegate;
	FOnTimelineVector ArmUpdateDelegate;
	FOnTimelineEvent TimelineFinishDelegate;

	// 조준 포스트 프로세스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPostProcessComponent* PostProcessComponent;
	UPROPERTY()
	UMaterialInstanceDynamic* DynMAimScreen;
	float AimMaterialAlpha;
};
