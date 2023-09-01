#pragma once

#include "CoreMinimal.h"
#include "AI/Boss/BossSpellBase.h"
#include "Frostbolt.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API AFrostbolt : public ABossSpellBase
{
	GENERATED_BODY()
	
public:
	AFrostbolt();

	USphereComponent* GetSphereComponent();

	void SetTargetActor(AActor* _TargetActor);
	void ShotFrostbolt();

	UFUNCTION()
	void FrostboltBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void ParticleSystemFinished(UParticleSystemComponent* _ParticleSystemComponent);


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime)override;

private:
	UFUNCTION()
	void DestroyProjectile(AActor* _Destroy);

	void AimTarget(float _DeltaTime);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor>DeathCreateObject = nullptr;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComponent = nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement = nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleComponent = nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ShotParticleComponent = nullptr;

	bool Shot;

	AActor* TargetActor;
};
