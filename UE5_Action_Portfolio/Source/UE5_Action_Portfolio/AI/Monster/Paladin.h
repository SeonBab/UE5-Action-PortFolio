#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalAICharacter.h"
#include "Global/Data/MonsterData.h"
#include "Paladin.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API APaladin : public AGlobalAICharacter
{
	GENERATED_BODY()

public:
	APaladin();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

private:
	const struct FMonsterData* CurMonsterData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataName = "Paladin";
};
