#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetCrosshairOnOff(bool _Value);
	UFUNCTION(BlueprintCallable)
	void SetBossInfoOnOff(bool _Value);
	UFUNCTION(BlueprintCallable)
	void SetMainCharHpBarOnOff(bool _Value);
	UFUNCTION(BlueprintCallable)
	void SetWeaponSlotOnOff(bool _Value);

protected:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility CrosshairOnOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility BossInfoOnOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility MainCharHpBarOnOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility WeaponSlotOnOff;
};
