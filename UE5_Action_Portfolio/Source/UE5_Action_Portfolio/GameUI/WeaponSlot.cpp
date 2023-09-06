#include "GameUI/WeaponSlot.h"
#include "Components/Image.h"
#include "Global/GlobalGameInstance.h"

void UWeaponSlot::SetWpSlotIcon(EWeaponType _WpType)
{
	UTexture* Texture = *(WpIcon.Find(_WpType));

	if (nullptr == Texture)
	{
		return;
	}

	//WpSlotIcon->SetBrushFromTexture();

	WeaponIconOnOff = ESlateVisibility::Visible;
}

void UWeaponSlot::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponIconOnOff = ESlateVisibility::Hidden;

	WpSlotBack = Cast<UImage>(GetWidgetFromName(TEXT("WeaponSlotBack")));
	WpSlotGrid = Cast<UImage>(GetWidgetFromName(TEXT("WeaponSlotGrid")));
	WpSlotIcon = Cast<UImage>(GetWidgetFromName(TEXT("WeaponSlotIcon")));

	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Inst)
	{
		return;
	}
	
	WpIcon = Inst->GetWeaponDataTMap();
}

void UWeaponSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
