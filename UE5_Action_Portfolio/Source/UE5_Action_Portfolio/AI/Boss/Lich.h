#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalAICharacter.h"
#include "Global/Data/BossData.h"
#include "Lich.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API ALich : public AGlobalAICharacter
{
	GENERATED_BODY()

public:
	ALich();
	UFUNCTION(BlueprintCallable)
	FName GetDataName();

	UCapsuleComponent* GetMeleeCapsuleComponent();
	UNiagaraComponent* GetMeleeNiagaraComponent();
	AActor* GetDarkBall();
	void SetDarkBall(AActor* _Actor);
	TArray<AActor*>& GetFrostboltArray();
	void SetNullFrostboltArray(int _Index);

	void Destroyed() override;

	void SetPhase(int _Phase);
	int GetPhase();

	AActor* GetTargetActor();

	UFUNCTION(BlueprintImplementableEvent)
	void BpEventCallBossInfo();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MeleeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

private:
	const struct FBossData* CurBossData;

	UPROPERTY(Category = "Melee", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* MeleeCapsuleComponent;
	UPROPERTY(Category = "Melee", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* MeleeNiagaraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataName;

	UPROPERTY()
	AActor* DarkBall;
	UPROPERTY()
	TArray<AActor*> FrostboltArray;
	
	float MeleeDamage;

	int MaxPhase;

	// 천천히 변하게 하는 disintegrate 효과
	// 머티리얼을 변경 할 방법 2가지
	// 데이터 테이블에 머리티얼을 저장해서 직접 스켈레탈 메쉬컴포넌트에 SetMaterial을 사용
	//USkeletalMeshComponent* PTr = nullptr;
	//PTr->SetMaterial(0, )
	// 머티리얼을 블루프린트에서 변수로 저장해 Array에서 하나씩 불러와 사용
	//TArray<UMat>
};
