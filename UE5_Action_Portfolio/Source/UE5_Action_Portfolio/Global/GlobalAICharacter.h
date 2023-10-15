#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GlobalAnimInstance.h"
#include "GlobalAICharacter.generated.h"

class UAnimMontage;
class UBehaviorTree;
class UBlackboardComponent;
class UWidgetComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API AGlobalAICharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	AGlobalAICharacter();
	UBehaviorTree* GetBehaviorTree();
	UBlackboardComponent* GetBlackboardComponent();

	UFUNCTION(BlueprintCallable)
	void SetAILockOnMarkOnOff(bool _Value);

	virtual void AIInit();

	// 태그
	void SetActorTypeTag(FName _Tag);
	FName GetActorTypeTag();
	void SetAttackTypeTag(FName _Tag);
	FName GetAttackTypeTag();

	// 패리
	bool GetParrybool();
	void SetParrybool(bool _Value);

	// 무적
	UFUNCTION(BlueprintCallable)
	bool GetIsInvincibility();
	UFUNCTION(BlueprintCallable)
	void SetIsInvincibility(bool _Value);

	// 체력
	UFUNCTION(BlueprintCallable)
	void SetHP(float _HP);
	UFUNCTION(BlueprintCallable)
	float GetHP();
	UFUNCTION(BlueprintCallable)
	void SetMaxHP(float _MaxHP);
	UFUNCTION(BlueprintCallable)
	float GetMaxHP();

	// 상태
	template<typename EnumType>
	UFUNCTION(BlueprintCallable)
		EnumType GetAnimState()
	{
		return static_cast<EnumType>(Animstate);
	}
	UFUNCTION(BlueprintCallable)
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

	// 몽타주
	template<typename EnumType>
	UAnimMontage* GetAnimMontage(EnumType _Index)
	{
		return GetAnimMontage(static_cast<int>(_Index));
	}

	UAnimMontage* GetAnimMontage(int _Index)
	{
		UGlobalAnimInstance* GlobalAnimInstance = Cast<UGlobalAnimInstance>(GetMesh()->GetAnimInstance());

		if (nullptr == GlobalAnimInstance)
		{
			return nullptr;
		}

		TMap<int, UAnimMontage*> AllAnimations = GlobalAnimInstance->GetAllAnimations();

		if (false == AllAnimations.Contains(_Index))
		{
			return nullptr;
		}

		return AllAnimations[_Index];
	}

	// 오디오 컴포넌트
	UAudioComponent* GetAudioComponent();


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Animstate;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* LockOnMarkWidget;

	FName ActorTypeTag;
	FName AttackTypeTag;
	bool Parrybool;
	bool IsInvincibility;

	// 체력
	float HP;
	float MaxHP;

	// 오디오 컴포넌트
	UAudioComponent* AudioComponent;
};
