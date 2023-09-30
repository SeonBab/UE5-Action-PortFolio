#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	TArray<FString> Tips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FText Tip;
};
