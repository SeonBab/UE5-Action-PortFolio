#include "UI/LoadingWidget.h"
#include "Global/GlobalGameInstance.h"

void ULoadingWidget::NativeOnInitialized()
{
	Tips =
	{
		FString(TEXT("WASDŰ �Է����� �̵��մϴ�.")),
		FString(TEXT("ShiftŰ�� ���� �� �� �ֽ��ϴ�.")),
		FString(TEXT("����1, 2, 3Ű�� ���⸦ ���� �� �� �ֽ��ϴ�.")),
		FString(TEXT("Į�� ���и� �� ���¿��� EŰ�� ������ �и��� �����մϴ�.")),
		FString(TEXT("�и��� �÷��̾�� ���⸦ ��� ���͸� �����մϴ�."))
	};
}

void ULoadingWidget::NativeConstruct()
{
	int RandTipsIndex = UGlobalGameInstance::MainRandom.RandRange(0, Tips.Num());

	Tip = FText::FromString(Tips[RandTipsIndex]);
}

void ULoadingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	
}
