#pragma once

#include "CoreMinimal.h"
#include "AI/AIWeaponCharacter.h"
#include "Global/Data/MonsterData.h"
#include "CloneMonster.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API ACloneMonster : public AAIWeaponCharacter
{
	GENERATED_BODY()

public:
	ACloneMonster();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

private:
	const struct FMonsterData* CurMonsterData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataName;
};
