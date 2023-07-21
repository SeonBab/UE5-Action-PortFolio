#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/WeaponData.h"
#include "WeaponActor.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponActor();

protected:
	virtual void BeginPlay() override;

public:	

	//여기에서 메쉬 생성?

};
