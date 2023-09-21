
#include "Global/GlobalBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameUI/GameUIHUD.h"
#include "GameUI/MainWidget.h"

void UGlobalBlueprintFunctionLibrary::MainUIOnOff(ESlateVisibility _Value)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(GetCurrentWorld(), 0);

	if (false == IsValid(Controller))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	AGameUIHUD* HUD = Cast<AGameUIHUD>(Controller->GetHUD());

	if (false == IsValid(HUD))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UMainWidget* MainWidget = HUD->GetMainWidget();

	if (false == IsValid(MainWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	MainWidget->SetVisibility(_Value);

	return;
}

UWorld* UGlobalBlueprintFunctionLibrary::GetCurrentWorld()
{
	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		if (nullptr == Context.World())
		{
			continue;
		}

		EWorldType::Type GetWorldType = Context.WorldType;

		switch (GetWorldType)
		{
		case EWorldType::None:
			break;
		case EWorldType::Game:
			break;
		case EWorldType::Editor:
			break;
		case EWorldType::PIE:
			return Context.World();
			break;
		case EWorldType::EditorPreview:
			break;
		case EWorldType::GamePreview:
			break;
		case EWorldType::GameRPC:
			break;
		case EWorldType::Inactive:
			break;
		default:
			break;
		}

	}

	return nullptr;
}
