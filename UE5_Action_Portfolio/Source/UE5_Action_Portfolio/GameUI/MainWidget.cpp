#include "GameUI/MainWidget.h"

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
