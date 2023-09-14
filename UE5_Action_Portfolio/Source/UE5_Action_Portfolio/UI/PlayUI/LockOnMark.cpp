#include "UI/PlayUI/LockOnMark.h"

void ULockOnMark::LockOnMarkOnOff(bool _Value)
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

void ULockOnMark::NativeConstruct()
{
	CrosshairOnOff = ESlateVisibility::Hidden;
}
