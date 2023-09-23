#pragma once

#include "CoreMinimal.h"
#include "AI/Monster/MonsterCharacter.h"
#include "Global/Enums.h"
#include "Global/FootIKComponent.h"
#include "AIWeaponCharacter.generated.h"

class UWeaponComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API AAIWeaponCharacter : public AMonsterCharacter
{
	GENERATED_BODY()

public:
	AAIWeaponCharacter();

	UFUNCTION(BlueprintCallable)
	UWeaponComponent* GetWeaponComponent();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

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

private:
	// FootIKTwoBone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UFootIKComponent* FootIKComponent;
	FFootIKOffset FootIKOffset;

	// WeaponAction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWeaponComponent* WeaponComponent;

};
