#pragma once

#include "CoreMinimal.h"
#include "AI/Monster/MonsterCharacter.h"
#include "SkeletonMonster.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API ASkeletonMonster : public AMonsterCharacter
{
	GENERATED_BODY()

public:
	ASkeletonMonster();

	virtual UCapsuleComponent* GetMeleeCapsuleComponent() override;
	virtual void AIInit() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION()
	void MeleeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataName;

	UPROPERTY(Category = "Melee", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* MeleeCapsuleComponent;
	float MeleeDamage;
};
