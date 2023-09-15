#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalBlueprintFunctionLibrary.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UGlobalBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "_Object"))
	static void MainUIOnOff(ESlateVisibility _Value);

	UFUNCTION(BlueprintCallable)
	static UWorld* GetCurrentWorld();
};
