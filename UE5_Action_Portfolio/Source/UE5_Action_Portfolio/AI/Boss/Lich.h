#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Lich.generated.h"
// ��� AI�۷ι��� �����ϱ�
UCLASS()
class UE5_ACTION_PORTFOLIO_API ALich : public ACharacter
{
	GENERATED_BODY()

public:
	ALich();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//const struct FMonsterData* CurMonsterData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataName = "Lich";

	FName ActorTypeTag;
	FName AttackTypeTag;
};
