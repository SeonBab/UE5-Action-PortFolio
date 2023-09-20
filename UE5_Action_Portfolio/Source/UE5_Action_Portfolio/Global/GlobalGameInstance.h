#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Enums.h"
#include "GlobalGameInstance.generated.h"

class UNiagaraSystem;
class UParticleSystem;
class UPaperSprite;
class UMaterial;

struct FAnimaitionData;
struct FWeaponData;
struct FMonsterData;
struct FBossData;

UCLASS()
class UE5_ACTION_PORTFOLIO_API UGlobalGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UGlobalGameInstance();
	~UGlobalGameInstance();

	FWeaponData* GetWeaponData(FName _Name);
	USkeletalMesh* GetWeaponMesh(FName _Name);
	TMap<EWeaponType, UPaperSprite*> GetWeaponDataTMap();
	FAnimaitionData* GetAllAnimaitionDatas(FName _Name);
	FMonsterData* GetMonsterData(FName _Name);
	FBossData* GetBossData(FName _Name);
	UNiagaraSystem* GetNiagaraAsset(FName _Name);
	UParticleSystem* GetParticleAsset(FName _Name);
	TSubclassOf<UObject> GetSubClass(FName _Name);
	TSubclassOf<UUserWidget> GetSubClassUserWidget(FName _Name);
	UMaterial* GetMaterialAsset(FName _Name);

public:
	static FRandomStream MainRandom;

private:
	UPROPERTY()
	UDataTable* WeaponDatas;
	UPROPERTY()
	UDataTable* Animations;
	UPROPERTY()
	UDataTable* MonsterDatas;
	UPROPERTY()
	UDataTable* BossDatas;
	UPROPERTY()
	UDataTable* NiagaraDatas;
	UPROPERTY()
	UDataTable* ParticleDatas;
	UPROPERTY()
	UDataTable* SubClassDatas;
	UPROPERTY()
	UDataTable* SubClassUserWidgetDatas;
	UPROPERTY()
	UDataTable* MaterialDatas;
};
