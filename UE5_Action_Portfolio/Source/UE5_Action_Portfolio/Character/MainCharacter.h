#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Global/Enums.h"
#include "Global/GlobalCharacter.h"
#include "Components/TimelineComponent.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

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
	void AimorBlock(float _Value);
	void LockOnTarget();
	void LookAtTarget(float _DeltaTime);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* _PlayerInputComponent) override;

private:
	UFUNCTION()
	void AimZoomTimelineUpdate(float _Value);
	UFUNCTION()
	void ArmTimelineUpdate(FVector _Value);
	UFUNCTION()
	void AimZoomOnFinish();
	void CameraLineTrace();


public:
	// 카메라
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* MainCameraSpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* MainCameraComponent;
	float BaseTurnRate;
	float BaseLookUpRate;
	float LockOnTargetRange = 2000.f;
	AActor* LockedOnTargetActor = nullptr;

private:
	FName ActorTag = TEXT("Player");
	FName AttackType = TEXT("PlayerAttack");
	bool IsLookAtTartget = false;

	FTimeline ChangeViewFTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* FOVCurveFloat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveVector* CameraSpringArmVector;
	FOnTimelineFloat FOVUpdateDelegate;
	FOnTimelineVector ArmUpdateDelegate;
	FOnTimelineEvent TimelineFinishDelegate;
	float AimZoomTimelineLength = 1.f;

};
