#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainWidget.h"
#include "GameUIHUD.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AGameUIHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	UMainWidget* GetMainWidget();
	
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	UMainWidget* MainWidget = nullptr;
};
