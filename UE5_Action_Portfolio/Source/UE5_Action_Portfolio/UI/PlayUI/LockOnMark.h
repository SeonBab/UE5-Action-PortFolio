#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockOnMark.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API ULockOnMark : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void LockOnMarkOnOff(bool _Value);

protected:
	void NativeConstruct() override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility CrosshairOnOff;
};
