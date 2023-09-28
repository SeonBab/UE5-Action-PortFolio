#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalAICharacter.h"
#include "MonsterCharacter.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AMonsterCharacter : public AGlobalAICharacter
{
	GENERATED_BODY()
public:
	AMonsterCharacter();

	virtual UCapsuleComponent* GetMeleeCapsuleComponent();

	UFUNCTION(BlueprintCallable)
	void SetMonsterHealthBarOnOff(bool _Value);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBarWidget;
};
