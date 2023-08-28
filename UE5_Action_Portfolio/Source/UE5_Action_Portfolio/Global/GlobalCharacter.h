#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GlobalAnimInstance.h"
#include "GlobalCharacter.generated.h"

class UAnimMontage;

UCLASS()
class UE5_ACTION_PORTFOLIO_API AGlobalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGlobalCharacter();
	void SetActorTypeTag(FName _Tag);
	FName GetActorTypeTag();
	void SetAttackTypeTag(FName _Tag);
	FName GetAttackTypeTag();
	void SetHP(float _HP);
	UFUNCTION(BlueprintCallable)
	float GetHP();
	void SetMaxHP(float _MaxHP);
	UFUNCTION(BlueprintCallable)
	float GetMaxHP();
	template<typename EnumType>
	EnumType GetAnimState()
	{
		return static_cast<EnumType>(Animstate);
	}
	int GetAnimState()
	{
		return Animstate;
	}
	template<typename EnumType>
	void SetAnimState(EnumType _AnimState)
	{
		Animstate = static_cast<int>(_AnimState);

	}
	void SetAnimState(int _AnimState);
	template<typename EnumType>
	void SetAllAnimation(const TMap<EnumType, UAnimMontage*>& _MapAnimation)
	{
		UGlobalAnimInstance* GlobalAnimInstance = Cast<UGlobalAnimInstance>(GetMesh()->GetAnimInstance());

		if (nullptr == GlobalAnimInstance)
		{
			return;
		}

		GlobalAnimInstance->SetAllAnimation<EnumType>(_MapAnimation);
	}

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Animstate = 0;

private:
	FName ActorTypeTag;
	FName AttackTypeTag;
	float HP;
	float MaxHP;
};
