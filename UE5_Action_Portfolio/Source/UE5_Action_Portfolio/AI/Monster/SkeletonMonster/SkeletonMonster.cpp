#include "AI/Monster/SkeletonMonster/SkeletonMonster.h"

ASkeletonMonster::ASkeletonMonster()
{
    PrimaryActorTick.bCanEverTick = true;

}

void ASkeletonMonster::BeginPlay()
{
    Super::BeginPlay();
}

void ASkeletonMonster::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);
}

float ASkeletonMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    return FinalDamage;
}