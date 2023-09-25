#include "AI/Boss/AI_BossCon.h"
#include "Global/GlobalAICharacter.h"
#include "Global/Enums.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAI_BossCon::AAI_BossCon()
{
	GetAISenseConfigSight()->SightRadius = 4000.f;
	GetAISenseConfigSight()->LoseSightRadius = 4500.f;
	GetAISenseConfigSight()->PeripheralVisionAngleDegrees = 360.f;
	GetAISenseConfigSight()->SetMaxAge(10.f);

	int Team = static_cast<int>(TeamType::BossTeam);
	AAIController::SetGenericTeamId(FGenericTeamId(Team));
}

void AAI_BossCon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AAI_BossCon::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);
}

void AAI_BossCon::BeginPlay()
{
	Super::BeginPlay();
}
