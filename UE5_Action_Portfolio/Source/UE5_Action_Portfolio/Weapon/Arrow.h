#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

class UProjectileMovementComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	AArrow();

	UFUNCTION()
	void ArrowBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void ArrowReRoad(ACharacter* _Character, FVector _JointPos, float _DeltaTime);
	void SetIsLocationAndRotation(bool _Value);
	void ArrowChangeCollision(FName _FName);
	void FireInDirection(FVector _FVector, FRotator _FRotator, AController* _Controller);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ArrowScene;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ArrowSkeletalMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	bool IsLocationAndRotation = true;
	UPROPERTY()
	AController* CurController;
};
