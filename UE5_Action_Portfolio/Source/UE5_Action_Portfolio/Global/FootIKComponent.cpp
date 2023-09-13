#include "Global/FootIKComponent.h"
#include "Global/GlobalCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UFootIKComponent::UFootIKComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFootIKComponent::SetBeginCapsuleSize(float _Value)
{
	BeginCapsuleSize = _Value;
}

void UFootIKComponent::SetFootSoket(FName _LFootSocket, FName _RFootSocket)
{
	LFootSocket = _LFootSocket;
	RFootSocket = _RFootSocket;
}

void UFootIKComponent::SetTraceDis(float _TraceDis)
{
	TraceDis = _TraceDis;
}

FFootIKOffset UFootIKComponent::GetFootIKOffset(AGlobalCharacter* _Character, float _DeltaTime)
{
	FFootIKOffset FootIKOffset{};

	if (nullptr == _Character || false == _Character->IsValidLowLevel())
	{
		return FootIKOffset;
	}

	UCapsuleComponent* CapsuleComponent = _Character->GetCapsuleComponent();

	if (nullptr == CapsuleComponent || false == CapsuleComponent->IsValidLowLevel())
	{
		return FootIKOffset;
	}

	TTuple<float, FVector> LeftTrace = FootIKLineTrace(_Character, CapsuleComponent, LFootSocket, TraceDis);
	TTuple<float, FVector> RightTrace = FootIKLineTrace(_Character, CapsuleComponent, RFootSocket, TraceDis);

	UpdateFootRotation(_DeltaTime, NormalToRotator(LeftTrace.Get<1>()), &FootIKOffset.FootIKRotatorLeft, 20.f);
	UpdateFootRotation(_DeltaTime, NormalToRotator(RightTrace.Get<1>()), &FootIKOffset.FootIKRotatorRight, 20.f);

	float HipOffsetValue = UKismetMathLibrary::Min(LeftTrace.Get<0>(), RightTrace.Get<0>());

	if (0.f < HipOffsetValue)
	{
		HipOffsetValue = 0.f;
	}

	UpdateFootOffset(_DeltaTime, HipOffsetValue, &FootIKOffset.HipOffset, 20.f);
	UpdateCapsuleHalfHeight(CapsuleComponent, _DeltaTime, HipOffsetValue, false);

	UpdateFootOffset(_DeltaTime, LeftTrace.Get<0>() - HipOffsetValue, &FootIKOffset.FootOffsetLeft, 20.f);
	UpdateFootOffset(_DeltaTime, RightTrace.Get<0>() - HipOffsetValue, &FootIKOffset.FootOffsetRight, 20.f);

	return FootIKOffset;
}

void UFootIKComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UFootIKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

TTuple<float, FVector> UFootIKComponent::FootIKLineTrace(AGlobalCharacter* _Character, UCapsuleComponent* _CapsuleComponent, FName _Socket, float _TraceDis)
{
	TTuple<float, FVector> ReturnValue;

	USkeletalMeshComponent* SkeletalMeshComponent = _Character->GetMesh();

	if (nullptr == SkeletalMeshComponent || false == SkeletalMeshComponent->IsValidLowLevel())
	{
		return ReturnValue;
	}

	FVector CharLocation = _Character->GetActorLocation();
	float CapsuleHalfHeight = _CapsuleComponent->GetScaledCapsuleHalfHeight();

	FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation(_Socket);
	FVector LineTraceStart = FVector(SocketLocation.X, SocketLocation.Y, CharLocation.Z - 40.f);
	FVector LineTraceEnd = FVector(SocketLocation.X, SocketLocation.Y, CharLocation.Z - CapsuleHalfHeight - _TraceDis);

	FHitResult HitResult;
	TArray<AActor*> ToIgnore;

	ToIgnore.Emplace(_Character->GetOwner());

	bool DebugCheck = true;
	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;

	if (true == DebugCheck)
	{
		DebugType = EDrawDebugTrace::ForOneFrame;
	}

	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), LineTraceStart, LineTraceEnd, TraceType, true, ToIgnore, DebugType, HitResult, true);

	float ImpactLength;
	FVector ImpactLocation(HitResult.ImpactNormal);

	if (true == HitResult.IsValidBlockingHit())
	{
		ImpactLength = (HitResult.ImpactPoint - HitResult.TraceEnd).Size();
		ImpactLength -= _TraceDis;
		ImpactLength += 5.f;
	}
	else
	{
		ImpactLength = 0.f;
	}

	ReturnValue.Key = ImpactLength;
	ReturnValue.Value = ImpactLocation;

	return ReturnValue;
}

FRotator UFootIKComponent::NormalToRotator(FVector _Vector)
{
	float FootAtan2_1 = UKismetMathLibrary::DegAtan2(_Vector.Y, _Vector.Z);
	float FootAtan2_2 = UKismetMathLibrary::DegAtan2(_Vector.X, _Vector.Z);

	// 값을 음수로
	FootAtan2_2 *= -1.f;

	FRotator ReturnRotator = FRotator(FootAtan2_2, 0.0f, FootAtan2_1);

	return ReturnRotator;
}

void UFootIKComponent::UpdateFootRotation(float _DeltaTime, FRotator _NormalToRotatorValue, FRotator* _FootRotatorValue, float _InterpSpeed)
{
	FRotator InterpRotator = UKismetMathLibrary::RInterpTo(*_FootRotatorValue, _NormalToRotatorValue, _DeltaTime, _InterpSpeed);
	*_FootRotatorValue = InterpRotator;
}

void UFootIKComponent::UpdateCapsuleHalfHeight(UCapsuleComponent* _CapsuleComponent, float _DeltaTime, float _HipsShifs, bool _ResetDefault)
{
	float CapsuleHalf = 0.0f;

	if (true == _ResetDefault)
	{
		CapsuleHalf = BeginCapsuleSize;
	}
	else
	{
		float HalfAbsSize = UKismetMathLibrary::Abs(_HipsShifs);
		CapsuleHalf = BeginCapsuleSize - HalfAbsSize;
	}

	float InterpValue = UKismetMathLibrary::FInterpTo(_CapsuleComponent->GetScaledCapsuleHalfHeight(), CapsuleHalf, _DeltaTime, 13.f);

	_CapsuleComponent->SetCapsuleHalfHeight(InterpValue, true);
}

void UFootIKComponent::UpdateFootOffset(float _DeltaTime, float _TargetValue, float* _EffectorValue, float _InterpSpeed)
{
	float InterpValue = UKismetMathLibrary::FInterpTo(*_EffectorValue, _TargetValue, _DeltaTime, _InterpSpeed);
	*_EffectorValue = InterpValue;
}

