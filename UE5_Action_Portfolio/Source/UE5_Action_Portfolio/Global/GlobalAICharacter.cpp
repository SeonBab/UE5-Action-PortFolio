#include "Global/GlobalAICharacter.h"
#include "Global/GlobalGameInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "UI/PlayUI/LockOnMark.h"
#include "AI/AICon.h"

AGlobalAICharacter::AGlobalAICharacter()
{
    Animstate = 0;
    ActorTypeTag = TEXT("");
    AttackTypeTag = TEXT("");
    Parrybool = false;
    IsInvincibility = false;
    HP = 0.f;
    MaxHP = 0.f;

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(GetMesh());

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

void AGlobalAICharacter::SetAILockOnMarkOnOff(bool _Value)
{
    if (false == IsValid(LockOnMarkWidget))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    UUserWidget* Widget = LockOnMarkWidget->GetWidget();
    ULockOnMark* LockOnMark= Cast<ULockOnMark>(Widget);

    if (false == IsValid(LockOnMark))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    LockOnMark->SetLockOnMarkOnOff(_Value);
}

void AGlobalAICharacter::AIInit()
{
    // 상속된 AI가 오버라이드 해서 사용한다.
}

void AGlobalAICharacter::SetActorTypeTag(FName _Tag)
{
    ActorTypeTag = _Tag;
}

FName AGlobalAICharacter::GetActorTypeTag()
{
    return ActorTypeTag;
}

void AGlobalAICharacter::SetAttackTypeTag(FName _Tag)
{
    AttackTypeTag = _Tag;
}

FName AGlobalAICharacter::GetAttackTypeTag()
{
    return AttackTypeTag;
}

bool AGlobalAICharacter::GetParrybool()
{
    return Parrybool;
}

void AGlobalAICharacter::SetParrybool(bool _Value)
{
    Parrybool = _Value;
}

bool AGlobalAICharacter::GetIsInvincibility()
{
    return IsInvincibility;
}

void AGlobalAICharacter::SetIsInvincibility(bool _Value)
{
    IsInvincibility = _Value;
}

void AGlobalAICharacter::SetHP(float _HP)
{
    HP = _HP;
}

float AGlobalAICharacter::GetHP()
{
    return HP;
}

void AGlobalAICharacter::SetMaxHP(float _MaxHP)
{
    MaxHP = _MaxHP;
}

float AGlobalAICharacter::GetMaxHP()
{
    return MaxHP;
}

void AGlobalAICharacter::SetAnimState(int _AnimState)
{
    Animstate = _AnimState;
}

UAudioComponent* AGlobalAICharacter::GetAudioComponent()
{
    return AudioComponent;
}

void AGlobalAICharacter::BeginPlay()
{
    Super::BeginPlay();

    Tags.Add(ActorTypeTag);
    Tags.Add(AttackTypeTag);

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

    AController* CurController =GetController();

    if (false == IsValid(CurController))
    {
        SpawnDefaultController();
    }
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
