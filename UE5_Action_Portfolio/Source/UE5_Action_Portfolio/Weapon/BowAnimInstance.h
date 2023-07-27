#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BowAnimInstance.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UBowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	void InitializeAnimation();
	void NativeBeginPlay();
	void NativeUpdateAnimation();
	const bool GetBowChordCheck();
	void SetBowChordCheck(bool _Value);
	void SetHandTransform(const FVector _HandTransform);

public:

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool BowChordCheck = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	FVector HandTransform = FVector::ZeroVector;
};
