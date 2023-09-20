#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/TitleUI/TitleWidget.h"
#include "TitleUIHUD.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API ATitleUIHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	UTitleWidget* GetTitleWidget();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	UTitleWidget* TitleWidget;
};
