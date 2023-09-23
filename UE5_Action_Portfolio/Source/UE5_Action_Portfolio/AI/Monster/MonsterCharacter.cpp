#include "AI/Monster/MonsterCharacter.h"
#include "Global/GlobalGameInstance.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameUI/HealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/PlayerCameraManager.h"

AMonsterCharacter::AMonsterCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPInfoWidget"));
    HealthBarWidget->SetWorldLocation({ 0.f, 0.f, 100.f });
    HealthBarWidget->SetupAttachment(GetCapsuleComponent());
    HealthBarWidget->SetWidgetSpace(EWidgetSpace::World);
    HealthBarWidget->SetDrawSize({ 100.f, 10.f });

}

void AMonsterCharacter::SetMonsterHealthBarOnOff(bool _Value)
{
    if (false == IsValid(HealthBarWidget))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    UUserWidget* Widget = HealthBarWidget->GetWidget();
    UHealthBar* WBPHealthBarWidget = Cast<UHealthBar>(Widget);

    if (false == IsValid(WBPHealthBarWidget))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    WBPHealthBarWidget->SetHealthBarOnOff(_Value);
}

void AMonsterCharacter::BeginPlay()
{
    Super::BeginPlay();

    UGlobalGameInstance* Instance = GetGameInstance<UGlobalGameInstance>();

    if (false == IsValid(Instance))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    TSubclassOf<UUserWidget> WBPHealthBar = Instance->GetSubClassUserWidget(TEXT("HealthBar"));

    if (false == IsValid(WBPHealthBar))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    HealthBarWidget->SetWidgetClass(WBPHealthBar);
}

void AMonsterCharacter::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);

    APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

    if (false == IsValid(PlayerCameraManager))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    if (false == IsValid(HealthBarWidget))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    // HealthBarWidget을 플레이어의 카메라에게 회전시킨다
    FVector CameraVector = PlayerCameraManager->GetCameraLocation();
    FVector WidgetVecotr = HealthBarWidget->GetComponentLocation();

    FRotator LookCameraRot = UKismetMathLibrary::FindLookAtRotation(WidgetVecotr, CameraVector);

    HealthBarWidget->SetWorldRotation(LookCameraRot);

    // HealthBarWidget에 HP를 반영한다
    
    UUserWidget* Widget = HealthBarWidget->GetWidget();
    UHealthBar* WBPHealthBarWidget = Cast<UHealthBar>(Widget);

    if (false == IsValid(WBPHealthBarWidget))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    WBPHealthBarWidget->SetHPInfo(GetHP(), GetMaxHP());
}

float AMonsterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    return FinalDamage;
}