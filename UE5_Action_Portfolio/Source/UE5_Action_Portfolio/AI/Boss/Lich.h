#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Lich.generated.h"
// 상속 AI글로벌로 변경하기
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
