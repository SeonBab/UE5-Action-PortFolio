// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GlobalAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Global/GlobalAIController.h"

UBehaviorTree* AGlobalAICharacter::GetBehaviorTree()
{
    return BehaviorTree;
}

UBlackboardComponent* AGlobalAICharacter::GetBlackboardComponent()
{
    if (nullptr == BlackboardComponent)
    {
        AGlobalAIController* AICon = GetController<AGlobalAIController>();
        
        if (nullptr == AICon)
        {
            return nullptr;
        }

        BlackboardComponent = AICon->GetBlackboardComponent();
    }

    return BlackboardComponent;
}
