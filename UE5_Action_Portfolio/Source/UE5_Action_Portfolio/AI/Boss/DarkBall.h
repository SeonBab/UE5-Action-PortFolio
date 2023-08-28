#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "DarkBall.generated.h"



UCLASS()
class UE5_ACTION_PORTFOLIO_API ADarkBall : public AActor
{
	GENERATED_BODY()
	
public:	
	ADarkBall();

	void SetDeathCheck(bool _Value);
	void SetSpeed(float _Value);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime)override;

private:
	UFUNCTION()
	void DestroyProjectile(AActor* _Destroy);

public:	
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Speed = 0.f;
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DeathTime = 0.f;
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor>DeathCreateObject = nullptr;

		
private:
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComponent = nullptr;
	bool DeathCheck = false;
};
