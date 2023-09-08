#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "EntryTitleScreen.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UEntryTitleScreen : public UUserWidgetBlueprint
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void PlayPressAnyButten();

private:

};
