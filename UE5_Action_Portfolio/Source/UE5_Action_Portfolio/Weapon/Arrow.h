#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Arrow.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	AArrow();

	UFUNCTION()
	void ArrowBeginOverlap();
	void ArrowReRoad(ACharacter* _Character, FVector _JointPos, float _DeltaTime);
	void SetIsRotation(bool _Value);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//class USphereComponent* SphereComponent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ArrowSkeletalMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	bool IsRotation = false;

};
