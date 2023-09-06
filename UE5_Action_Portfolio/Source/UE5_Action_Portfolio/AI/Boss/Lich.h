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

	// õõ�� ���ϰ� �ϴ� disintegrate ȿ��
	// ��Ƽ������ ���� �� ��� 2����
	// ������ ���̺� �Ӹ�Ƽ���� �����ؼ� ���� ���̷�Ż �޽�������Ʈ�� SetMaterial�� ���
	//USkeletalMeshComponent* PTr = nullptr;
	//PTr->SetMaterial(0, )
	// ��Ƽ������ �������Ʈ���� ������ ������ Array���� �ϳ��� �ҷ��� ���
	//TArray<UMat>
};
