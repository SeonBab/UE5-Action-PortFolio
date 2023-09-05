#include "GameUI/MainWidget.h"
#include "GameUI/HealthBar.h"
#include "GameUI/BossInfo.h"

void UMainWidget::SetCrosshairOnOff(bool _Value)
{
	if (true == _Value)
	{
		CrosshairOnOff = ESlateVisibility::Visible;
	}
	else if (false == _Value)
	{
		CrosshairOnOff = ESlateVisibility::Hidden;
	}
}

void UMainWidget::SetBossHpBar(bool _Value)
{
	if (true == _Value)
	{
		BossHpBar = ESlateVisibility::Visible;
	}
	else if (false == _Value)
	{
		BossHpBar = ESlateVisibility::Hidden;
	}
}

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CrosshairOnOff = ESlateVisibility::Hidden;
	BossHpBar = ESlateVisibility::Hidden;

	CharacterHPBar = Cast<UHealthBar>(GetWidgetFromName(TEXT("")));
	BossInfo = Cast<UBossInfo>(GetWidgetFromName(TEXT("")));
}

void UMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
