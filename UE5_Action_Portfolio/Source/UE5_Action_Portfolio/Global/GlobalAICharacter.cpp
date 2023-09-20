#include "Global/GlobalAICharacter.h"
#include "Global/GlobalGameInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "UI/PlayUI/LockOnMark.h"
#include "AI/AICon.h"

AGlobalAICharacter::AGlobalAICharacter()
{
    LockOnMarkWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnMarkWidget"));
    LockOnMarkWidget->SetupAttachment(GetCapsuleComponent());
    LockOnMarkWidget->SetWorldLocation({ 0.f, 0.f, 35.f});
    LockOnMarkWidget->SetWidgetSpace(EWidgetSpace::Screen);
    LockOnMarkWidget->SetDrawSize({ 30.f, 30.f });
}

UBehaviorTree* AGlobalAICharacter::GetBehaviorTree()
{
    return BehaviorTree;
}

UBlackboardComponent* AGlobalAICharacter::GetBlackboardComponent()
{
    if (nullptr == BlackboardComponent)
    {
        AAICon* AICon = GetController<AAICon>();
        
        if (false == IsValid(AICon))
        {
            UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
            return nullptr;
        }

        BlackboardComponent = AICon->GetBlackboardComponent();
    }

    if (false == IsValid(BlackboardComponent))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return nullptr;
    }

    return BlackboardComponent;
}

void AGlobalAICharacter::LockOnMarkOnOff(bool _Value)
{
    UUserWidget* Widget = LockOnMarkWidget->GetWidget();
    ULockOnMark* LockOnMark= Cast<ULockOnMark>(Widget);

    if (false == IsValid(LockOnMark))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    LockOnMark->LockOnMarkOnOff(_Value);
}

void AGlobalAICharacter::BeginPlay()
{
    Super::BeginPlay();

    UGlobalGameInstance* Instance = GetGameInstance<UGlobalGameInstance>();

    if (false == IsValid(Instance))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    TSubclassOf<UUserWidget> WBPLockOnMark = Instance->GetSubClassUserWidget(TEXT("LockOnMark"));

    if (false == IsValid(WBPLockOnMark))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    LockOnMarkWidget->SetWidgetClass(WBPLockOnMark);
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
