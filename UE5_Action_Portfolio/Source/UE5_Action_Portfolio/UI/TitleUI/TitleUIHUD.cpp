#include "UI/TitleUI/TitleUIHUD.h"
#include "Global/GlobalGameInstance.h"

UTitleWidget* ATitleUIHUD::GetTitleWidget()
{
	return TitleWidget;
}

void ATitleUIHUD::BeginPlay()
{
	Super::BeginPlay();

	UGlobalGameInstance* Instance = GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	TSubclassOf<UUserWidget> TitleWidgetClass = Instance->GetSubClassUserWidget(TEXT("TitleWidget"));

	UUserWidget* Widget = CreateWidget(GetWorld(), TitleWidgetClass);
	TitleWidget = Cast<UTitleWidget>(Widget);

	if (false == IsValid(TitleWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	TitleWidget->AddToViewport();
}

void ATitleUIHUD::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}
