#include "AI/Monster/AIMonsterCon.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Global/GlobalAICharacter.h"
#include "Global/Enums.h"

AAIMonsterCon::AAIMonsterCon()
{
	GetAISenseConfigSight()->SightRadius = 2000.f;
	GetAISenseConfigSight()->LoseSightRadius = 3500.f;
	GetAISenseConfigSight()->PeripheralVisionAngleDegrees = 90.f;
	GetAISenseConfigSight()->SetMaxAge(5.f);

	int Team = static_cast<int>(TeamType::MonsterTeam);
	AAIController::SetGenericTeamId(FGenericTeamId(Team));
}

void AAIMonsterCon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AAIMonsterCon::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);
}

void AAIMonsterCon::BeginPlay()
{
	Super::BeginPlay();
}
