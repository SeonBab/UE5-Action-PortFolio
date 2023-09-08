#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/TitleScreen/TitleScreenWidget.h"
#include "TitleScreenUIHUD.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API ATitleScreenUIHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	UTitleScreenWidget* GetTitleScreenWidget();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	UTitleScreenWidget* TitleScreenWidget;
};
