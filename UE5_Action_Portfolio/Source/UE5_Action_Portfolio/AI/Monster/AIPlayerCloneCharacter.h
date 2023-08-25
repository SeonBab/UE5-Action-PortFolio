#pragma once

#include "CoreMinimal.h"
#include "Character/WeaponCharacter.h"
#include "AIPlayerCloneCharacter.generated.h"

class UBehaviorTree;
class UBlackboardComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API AAIPlayerCloneCharacter : public AWeaponCharacter
{
	GENERATED_BODY()
	
public:
	UBehaviorTree* GetBehaviorTree();
	UBlackboardComponent* GetBlackboardComponent();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class UBlackboardComponent* BlackboardComponent;
};
