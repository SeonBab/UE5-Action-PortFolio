#pragma once

#include "CoreMinimal.h"
#include "AI/Boss/BossSpellBase.h"
#include "Tornado.generated.h"

class UCapsuleComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API ATornado : public ABossSpellBase
{
	GENERATED_BODY()
	
public:	
	ATornado();

	UCapsuleComponent* GetCapsuleComponent();
	void SetAttackType(FName _AttackType);
	void SetTargetCharacter(ACharacter* _TargetCharacter);

	UFUNCTION()
	void TornadoBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void TornadoEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent = nullptr;
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDecalComponent* DecalComponent = nullptr;

	ACharacter* TargetCharacter = nullptr;
	AActor* OverlapOtherActor = nullptr;

	bool TornadoSpawnCheck = false;

	float HitTime = 0.f;
	float DecalTime = 0.f;
	float DecalFadeStartDelay = 0.f;
	float DecalFadeDuration = 0.f;

	FName AttackType;
};
