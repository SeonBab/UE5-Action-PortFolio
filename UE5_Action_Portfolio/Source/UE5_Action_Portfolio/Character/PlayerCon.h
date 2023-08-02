// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerCon.generated.h"

/**
 * 
 */
UCLASS()
class UE5_ACTION_PORTFOLIO_API APlayerCon : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	virtual FGenericTeamId GetGenericTeamId() const override;

private: 
	FGenericTeamId TeamId = FGenericTeamId(0);
};
