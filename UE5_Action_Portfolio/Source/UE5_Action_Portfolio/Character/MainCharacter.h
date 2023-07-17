#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Global/Enums.h"
#include "MainCharacter.generated.h"


UCLASS()
class UE5_ACTION_PORTFOLIO_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	// 조작
	void ZoomIn();
	void ZoomOut();
	void MoveForward(float _Value);
	void MoveRight(float _Value);
	void RollorRun(float _Value);
	void JumpAction();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* _PlayerInputComponent) override;

public:
	// 카메라
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* MainCameraSpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* MainCameraComponent;
	float BaseTurnRate;
	float BaseLookUpRate;

	// 무기
	UPROPERTY(BlueprintReadWrite)
	class UWeaponAction* CurWeapon;
	MainCharacterAnimState* AnimState;
};
