#include "GameUI/WeaponSlot.h"
#include "Components/Image.h"
#include "PaperSprite.h"
#include "Global/GlobalGameInstance.h"

void UWeaponSlot::SetWpSlotIcon(EWeaponType _WpType)
{
	UPaperSprite* PaperSprite = *(WpIcon.Find(_WpType));

	if (nullptr == PaperSprite)
	{
		return;
	}

	WpSlotIcon->SetBrushFromAtlasInterface(PaperSprite);

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
