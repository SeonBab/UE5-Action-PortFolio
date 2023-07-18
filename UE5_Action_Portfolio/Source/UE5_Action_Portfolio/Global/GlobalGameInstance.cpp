// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalGameInstance.h"
#include "AnimaitionData.h"

UGlobalGameInstance::UGlobalGameInstance()
{
	{
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrint/Global/DT_AnimaitionData.DT_AnimaitionData'");
		ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

		if (DataTable.Succeeded())
		{
			AllAnimations = DataTable.Object;
		}
	}

	{
		// 무기 데이터
	}
}

UGlobalGameInstance::~UGlobalGameInstance()
{
}

//TMap<CharacterAnimState, UAnimMontage*> UGlobalGameInstance::GetAllAnimations(FName Name)
//{
//	check(nullptr != AllAnimations)
//
//	FAnimaitionData* FindTable = AllAnimations->FindRow<FAnimaitionData>(Name, Name.ToString());
//
//	check(nullptr != FindTable)
//
//	return FindTable->AllAnimations;
//}

FAnimaitionData* UGlobalGameInstance::GetAnimaitionData(FName Name)
{
	check(nullptr != AllAnimations)

	FAnimaitionData* FindTable = AllAnimations->FindRow<FAnimaitionData>(Name, Name.ToString());

	check(nullptr != FindTable)

	return FindTable;
}
