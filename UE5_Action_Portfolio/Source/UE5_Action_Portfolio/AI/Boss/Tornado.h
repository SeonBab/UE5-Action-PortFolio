#pragma once

#include "CoreMinimal.h"
#include "AI/Boss/BossSpellBase.h"
#include "Tornado.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API ATornado : public ABossSpellBase
{
	GENERATED_BODY()
	
public:	
	ATornado();

	UFUNCTION()
	void TornadoBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

};
