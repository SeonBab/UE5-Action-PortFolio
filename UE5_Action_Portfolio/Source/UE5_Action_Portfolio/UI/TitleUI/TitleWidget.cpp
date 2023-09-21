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

void UTitleWidget::SetLogoWidgetOnOff(bool _Value)
{
	if (true == _Value)
	{
		LogoWidgetOnOff = ESlateVisibility::Visible;
	}
	else if (false == _Value)
	{
		LogoWidgetOnOff = ESlateVisibility::Hidden;
	}
}

void UTitleWidget::SetMenuButtenWidgetOnOff(bool _Value)
{
	if (true == _Value)
	{
		MenuButtenWidgetOnOff = ESlateVisibility::Visible;
	}
	else if (false == _Value)
	{
		MenuButtenWidgetOnOff = ESlateVisibility::Hidden;
	}
}

void UTitleWidget::NativeConstruct()
{
	PressAnyKeyWidgetOnOff = ESlateVisibility::Visible;
	LogoWidgetOnOff = ESlateVisibility::Hidden;
	MenuButtenWidgetOnOff = ESlateVisibility::Hidden;
}

void UTitleWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}
