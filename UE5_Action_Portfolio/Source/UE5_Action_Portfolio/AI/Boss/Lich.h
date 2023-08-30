#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalAICharacter.h"
#include "Global/Data/BossData.h"
#include "Lich.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API ALich : public AGlobalAICharacter
{
	GENERATED_BODY()

public:
	ALich();
	void SetDarkBall(AActor* _Actor);
	AActor* GetDarkBall();
	UCapsuleComponent* GetMeleeCapsuleComponent();
	UNiagaraComponent* GetNiagaraComponent();
	void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MeleeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

private:
	const struct FBossData* CurBossData;

	UPROPERTY(Category = "Melee", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* MeleeCapsuleComponent = nullptr;
	UPROPERTY(Category = "Melee", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataName = "Lich";

	AActor* DarkBall = nullptr;

	float MeleeDamage = 0.f;
};
