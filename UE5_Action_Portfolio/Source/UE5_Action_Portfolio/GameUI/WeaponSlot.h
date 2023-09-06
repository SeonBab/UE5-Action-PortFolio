#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Global/Enums.h"
#include "WeaponSlot.generated.h"

class UImage;

UCLASS()
class UE5_ACTION_PORTFOLIO_API UWeaponSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetWpSlotIcon(EWeaponType _WpType);

protected:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility WeaponIconOnOff;

	UImage* WpSlotBack;
	UImage* WpSlotGrid;
	UImage* WpSlotIcon;

	TMap<EWeaponType, UTexture*> WpIcon;
};
