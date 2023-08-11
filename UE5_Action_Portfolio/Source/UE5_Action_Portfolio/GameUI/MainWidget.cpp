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
