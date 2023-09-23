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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	
};
