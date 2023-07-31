#include "Global/GlobalAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AICon.h"

UBehaviorTree* AGlobalAICharacter::GetBehaviorTree()
{
    return BehaviorTree;
}

UBlackboardComponent* AGlobalAICharacter::GetBlackboardComponent()
{
    if (nullptr == BlackboardComponent)
    {
        AAICon* AICon = GetController<AAICon>();
        
        if (nullptr == AICon)
        {
            return nullptr;
        }

        BlackboardComponent = AICon->GetBlackboardComponent();
    }

    return BlackboardComponent;
}
