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

void UMainWidget::SetBossInfoOnOff(bool _Value)
{
	if (true == _Value)
	{
		BossInfoOnOff = ESlateVisibility::Visible;
	}
	else if (false == _Value)
	{
		BossInfoOnOff = ESlateVisibility::Hidden;
	}
}

void UMainWidget::SetMainCharHpBarOnOff(bool _Value)
{
	if (true == _Value)
	{
		MainCharHpBarOnOff = ESlateVisibility::Visible;
	}
	else if (false == _Value)
	{
		MainCharHpBarOnOff = ESlateVisibility::Hidden;
	}
}

void UMainWidget::SetWeaponSlotOnOff(bool _Value)
{
	if (true == _Value)
	{
		WeaponSlotOnOff = ESlateVisibility::Visible;
	}
	else if (false == _Value)
	{
		WeaponSlotOnOff = ESlateVisibility::Hidden;
	}
}

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CrosshairOnOff = ESlateVisibility::Hidden;
	BossInfoOnOff = ESlateVisibility::Hidden;
	MainCharHpBarOnOff = ESlateVisibility::Hidden;
	WeaponSlotOnOff = ESlateVisibility::Hidden;
}

void UMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
