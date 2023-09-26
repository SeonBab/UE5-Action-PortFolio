#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalCharacter.h"
#include "GlobalAICharacter.generated.h"

class UBehaviorTree;
class UBlackboardComponent;
class UWidgetComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API AGlobalAICharacter : public AGlobalCharacter
{
	GENERATED_BODY()
	
public:
	AGlobalAICharacter();
	UBehaviorTree* GetBehaviorTree();
	UBlackboardComponent* GetBlackboardComponent();

	UFUNCTION(BlueprintCallable)
	void SetAILockOnMarkOnOff(bool _Value);

	virtual void AIInit();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* LockOnMarkWidget;
};
