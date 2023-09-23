#include "UI/PlayUI/LockOnMark.h"

void ULockOnMark::SetLockOnMarkOnOff(bool _Value)
{
	if (true == _Value)
	{
		LockOnMarkOnOff = ESlateVisibility::Visible;
	}
	else if (false == _Value)
	{
		LockOnMarkOnOff = ESlateVisibility::Hidden;
	}
}

void ULockOnMark::NativeConstruct()
{
	LockOnMarkOnOff = ESlateVisibility::Hidden;
}
