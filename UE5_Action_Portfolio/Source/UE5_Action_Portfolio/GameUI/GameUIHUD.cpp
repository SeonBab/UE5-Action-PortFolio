#include "GameUI/GameUIHUD.h"

UMainWidget* AGameUIHUD::GetMainWidget()
{
	return MainWidget;
}

void AGameUIHUD::BeginPlay()
{
	Super::BeginPlay();

	FSoftClassPath ClassPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/UI/WBP_MainWidget.WBP_MainWidget_C'"));
	TSubclassOf<UUserWidget> MainWidgetClass = ClassPath.TryLoadClass<UUserWidget>();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass);
	MainWidget = Cast<UMainWidget>(Widget);
	MainWidget->AddToViewport();
}

void AGameUIHUD::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}
