#include "AI/Monster/AIPlayerCloneCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AICon.h"

UBehaviorTree* AAIPlayerCloneCharacter::GetBehaviorTree()
{
    return BehaviorTree;
}

UBlackboardComponent* AAIPlayerCloneCharacter::GetBlackboardComponent()
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

void AAIPlayerCloneCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AAIPlayerCloneCharacter::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);
}

float AAIPlayerCloneCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    return FinalDamage;
}
