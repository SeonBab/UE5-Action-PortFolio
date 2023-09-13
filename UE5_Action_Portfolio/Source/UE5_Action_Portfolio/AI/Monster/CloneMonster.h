#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalAICharacter.h"
#include "Global/Data/MonsterData.h"
#include "CloneMonster.generated.h"

class UWeaponComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API ACloneMonster : public AGlobalAICharacter
{
	GENERATED_BODY()

public:
	ACloneMonster();

	UFUNCTION(BlueprintCallable)
	UWeaponComponent* GetWeaponComponent();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

private:
	const struct FMonsterData* CurMonsterData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWeaponComponent* WeaponComponent;
};
