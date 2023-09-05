#include "GameUI/HealthBar.h"
#include "Math/Vector2D.h"

void UHealthBar::SetHPInfo(float _HP, float _MaxHP)
{
	float RenderScaleX = _HP / _MaxHP;
	FVector2D RenderScale({ RenderScaleX, 0.f });
}

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
