#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCrosshairOnOff(bool _Value);
	void SetBossHpBar(bool _Value);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility CrosshairOnOff = ESlateVisibility::Hidden;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility BossHpBar = ESlateVisibility::Hidden;
};
