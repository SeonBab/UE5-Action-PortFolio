#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Global/Enums.h"
#include "Global/GlobalCharacter.h"
#include "MainCharacter.generated.h"


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

public:
	// 카메라
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* MainCameraSpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* MainCameraComponent;
	float BaseTurnRate;
	float BaseLookUpRate;
	float LockOnTargetRange = 1500.f;
	AActor* LockedOnTargetActor = nullptr;

private:
	FName ActorType = TEXT("Player");

	bool IsLookAtTartget = false;

};
