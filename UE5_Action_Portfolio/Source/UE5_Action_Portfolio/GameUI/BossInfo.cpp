#include "GameUI/BossInfo.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameUI/HealthBar.h"

void UBossInfo::SetBossInfo(FName _InfoName, float _HP, float _MaxHP)
{
	if (nullptr == HPBar)
	{
		return;
	}

	HPBar->SetHPInfo(_HP, _MaxHP);

	FText NameText = FText::FromName(_InfoName);
	InfoName->SetText(NameText);
}

void UBossInfo::NativeConstruct()
{
	Super::NativeConstruct();

	InfoName = Cast<UTextBlock>(GetWidgetFromName(TEXT("BossName")));
	HPBar = Cast<UHealthBar>(GetWidgetFromName(TEXT("WBP_HealthBar")));	
}

void UBossInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
