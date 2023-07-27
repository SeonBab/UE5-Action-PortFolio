// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GlobalAICharacter.h"

UBehaviorTree* AGlobalAICharacter::GetBehaviorTree()
{
    return BehaviorTree;
}

UBlackboardComponent* AGlobalAICharacter::GetBlackboardComponent()
{
    if (nullptr == BlackboardComponent)
    {
        
    }

    return BlackboardComponent;
}
