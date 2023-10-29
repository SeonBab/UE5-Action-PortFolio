#include "AI/Monster/Clone/AIPlayerCloneCon.h"
#include "AI/Monster/Clone/CloneMonster.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAIPlayerCloneCon::AAIPlayerCloneCon()
{	
	GetAISenseConfigSight()->SightRadius = 4500.f;
	GetAISenseConfigSight()->LoseSightRadius = 5000.f;
	GetAISenseConfigSight()->PeripheralVisionAngleDegrees = 90.f;
	GetAISenseConfigSight()->SetMaxAge(8.f);

	int Team = static_cast<int>(TeamType::CloneTeam);
	AAIController::SetGenericTeamId(FGenericTeamId(Team));
}

void AAIPlayerCloneCon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AAIPlayerCloneCon::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);
}

void AAIPlayerCloneCon::BeginPlay()
{
	Super::BeginPlay();
}
