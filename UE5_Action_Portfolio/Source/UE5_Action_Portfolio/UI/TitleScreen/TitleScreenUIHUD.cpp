#include "UI/TitleScreen/TitleScreenUIHUD.h"

UTitleScreenWidget* ATitleScreenUIHUD::GetTitleScreenWidget()
{
	return TitleScreenWidget;
}

void ATitleScreenUIHUD::BeginPlay()
{
	Super::BeginPlay();

	FSoftClassPath ClassPath(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/UI/TitleScreen/BP_TitleScreenUIHUD.BP_TitleScreenUIHUD'"));
	TSubclassOf<UTitleScreenWidget> TitleScreenWidgetClass = ClassPath.TryLoadClass<UUserWidget>();

	UTitleScreenWidget* Widget = CreateWidget<UTitleScreenWidget>(GetWorld(), TitleScreenWidgetClass);
	TitleScreenWidget = Cast<UTitleScreenWidget>(Widget);
	TitleScreenWidget->AddToViewport();
}

void ATitleScreenUIHUD::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}
