#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossSpellBase.generated.h"

class UNiagaraComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API ABossSpellBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossSpellBase();

	void SetDeathCheck(bool _Value);
	bool GetDeathCheck();
	void SetSpeed(float _Value);
	void SetCurController(AController* _Controller);
	AController* GetCurController();
	UNiagaraComponent* GetNiagaraComponent();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Speed = 0.f;
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Damage = 0.f;
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float DeathTime = 0.f;

private:
	bool DeathCheck = false;

	AController* CurController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraComponent = nullptr;
};
