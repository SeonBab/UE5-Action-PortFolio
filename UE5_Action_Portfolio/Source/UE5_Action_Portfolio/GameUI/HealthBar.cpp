#include "GameUI/HealthBar.h"
#include "Components/Image.h"
#include "Math/Vector2D.h"

void UHealthBar::SetHPInfo(float _HP, float _MaxHP)
{
	float RenderScaleX = _HP / _MaxHP;
	FVector2D RenderScale({ RenderScaleX, 1.f });

	HPBar->SetRenderScale(RenderScale);
}

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	MaxHPBar = Cast<UImage>(GetWidgetFromName(TEXT("MaxHealthBar")));
	HPBar = Cast<UImage>(GetWidgetFromName(TEXT("HealthBar")));
}

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
