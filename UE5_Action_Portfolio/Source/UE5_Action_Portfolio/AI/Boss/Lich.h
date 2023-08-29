#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalAICharacter.h"
#include "Global/Data/BossData.h"
#include "Lich.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API ALich : public AGlobalAICharacter
{
	GENERATED_BODY()

public:
	ALich();
	void SetDarkBall(AActor* _Actor);
	AActor* GetDarkBall();

	void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

private:
	const struct FBossData* CurBossData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataName = "Lich";

	FName ActorTypeTag;
	FName AttackTypeTag;

	AActor* DarkBall = nullptr;


};
