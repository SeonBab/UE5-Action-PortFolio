#include "UI/LoadingWidget.h"
#include "Global/GlobalGameInstance.h"

void ULoadingWidget::NativeOnInitialized()
{
	Tips =
	{
		FString(TEXT("WASD키 입력으로 이동합니다.")),
		FString(TEXT("Shift키로 점프 할 수 있습니다.")),
		FString(TEXT("숫자1, 2, 3키로 무기를 변경 할 수 있습니다.")),
		FString(TEXT("칼과 방패를 든 상태에서 E키를 누르면 패리를 시전합니다.")),
		FString(TEXT("패링은 플레이어같이 무기를 드는 몬스터만 가능합니다."))
	};
}

void ULoadingWidget::NativeConstruct()
{
	int RandMax = Tips.Num() - 1;
	int RandTipsIndex = UGlobalGameInstance::MainRandom.RandRange(0, RandMax);

	Tip = FText::FromString(Tips[RandTipsIndex]);
}

void ULoadingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	
}
