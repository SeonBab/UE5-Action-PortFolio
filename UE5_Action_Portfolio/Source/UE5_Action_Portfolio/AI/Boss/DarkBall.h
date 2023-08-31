#pragma once

#include "CoreMinimal.h"
#include "AI/Boss/BossSpellBase.h"
#include "DarkBall.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API ADarkBall : public ABossSpellBase
{
	GENERATED_BODY()
	
public:	
	ADarkBall();

	USphereComponent* GetSphereComponent();
	void ShotDarkBall(FVector _FVector);

	UFUNCTION()
	void DarkBallBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime)override;

private:
	UFUNCTION()
	void DestroyProjectile(AActor* _Destroy);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor>DeathCreateObject = nullptr;
		
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComponent = nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
};
