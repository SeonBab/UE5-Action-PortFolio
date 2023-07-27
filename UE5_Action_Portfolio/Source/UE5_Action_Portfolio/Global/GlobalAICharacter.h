// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalCharacter.h"
#include "GlobalAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class UE5_ACTION_PORTFOLIO_API AGlobalAICharacter : public AGlobalCharacter
{
	GENERATED_BODY()
	
public:
	class UBehaviorTree* GetBehaviorTree();
	class UBlackboardComponent* GetBlackboardComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class UBlackboardComponent* BlackboardComponent;
};
