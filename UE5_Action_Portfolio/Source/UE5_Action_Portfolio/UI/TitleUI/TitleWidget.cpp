#include "UI/TitleUI/TitleWidget.h"

void UTitleWidget::SetPressAnyKeyWidgetOnOff(bool _Value)
{
	if (true == _Value)
	{
		PressAnyKeyWidgetOnOff = ESlateVisibility::Visible;
	}
	else if (false == _Value)
	{
		PressAnyKeyWidgetOnOff = ESlateVisibility::Hidden;
	}
}

void UTitleWidget::NativeConstruct()
{
	PressAnyKeyWidgetOnOff = ESlateVisibility::Visible;
}

void UTitleWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}
