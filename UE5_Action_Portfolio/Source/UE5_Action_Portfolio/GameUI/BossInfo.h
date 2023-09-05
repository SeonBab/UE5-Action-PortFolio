#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossInfo.generated.h"

class UTextBlock;
class UHealthBar;

UCLASS()
class UE5_ACTION_PORTFOLIO_API UBossInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetBossInfo(FName _InfoName, float _HP, float _MaxHP);

protected:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UTextBlock* InfoName;
	UHealthBar* HPBar;
};
