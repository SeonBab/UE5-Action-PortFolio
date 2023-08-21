#include "Global/GlobalAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Monster/AICon.h"

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

void AGlobalAICharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AGlobalAICharacter::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);
}

float AGlobalAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    return FinalDamage;
}
