#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UImage;

UCLASS()
class UE5_ACTION_PORTFOLIO_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHPInfo(float _HP, float _MaxHP);
	
protected:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UImage* HPBar;
	UImage* MaxHPBar;
};
