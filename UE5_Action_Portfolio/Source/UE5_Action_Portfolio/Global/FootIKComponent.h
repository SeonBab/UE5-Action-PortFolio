#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootIKComponent.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FFootIKOffset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		FRotator FootIKRotatorLeft = FRotator::ZeroRotator;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		FRotator FootIKRotatorRight = FRotator::ZeroRotator;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		float HipOffset;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		float FootOffsetLeft;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		float FootOffsetRight;
};

class AGlobalAICharacter;
class UCapsuleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_ACTION_PORTFOLIO_API UFootIKComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFootIKComponent();

	void SetBeginCapsuleSize(float _Value);
	void SetFootSoket(FName _LFootSocket, FName _RFootSocket);
	void SetTraceDis(float _TraceDis);
	FFootIKOffset GetFootIKOffset(AGlobalAICharacter* _Character, float _DeltaTime);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	TTuple<float, FVector> FootIKLineTrace(AGlobalAICharacter* _Character, UCapsuleComponent* _CapsuleComponent, FName _Socket, float _TraceDis);
	FRotator NormalToRotator(FVector _Vector);
	void UpdateFootRotation(float _DeltaTime, FRotator _NormalToRotatorValue, FRotator* _FootRotatorValue, float _InterpSpeed);
	void UpdateCapsuleHalfHeight(UCapsuleComponent* _CapsuleComponent, float _DeltaTime, float _HipsShifs, bool _ResetDefault);
	void UpdateFootOffset(float _DeltaTime, float _TargetValue, float* _EffectorValue, float _InterpSpeed);

//private:	
//	TTuple<float, FVector> FootIKLineTrace(AGlobalAICharacter* _Character, UCapsuleComponent* _CapsuleComponent, FName _Socket, float _TraceDis);
//	FRotator NormalToRotator(FVector _Vector);
//	void UpdateFootRotation(float _DeltaTime, FRotator _NormalToRotatorValue, FRotator* _FootRotatorValue, float _InterpSpeed);
//	void UpdateCapsuleHalfHeight(UCapsuleComponent* _CapsuleComponent, float _DeltaTime, float _HipsShifs, bool _ResetDefault);
//	void UpdateFootOffset(float _DeltaTime, float _TargetValue, float* _EffectorValue, float _InterpSpeed);
		
private:
	float BeginCapsuleSize;
	float TraceDis;
	FName LFootSocket;
	FName RFootSocket;
};