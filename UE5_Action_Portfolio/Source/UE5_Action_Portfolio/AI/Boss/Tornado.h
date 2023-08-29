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

	ACharacter* TargetCharacter = nullptr;
	AActor* OverlapOtherActor = nullptr;

	float HitTimeCheck = 0.f;
};
