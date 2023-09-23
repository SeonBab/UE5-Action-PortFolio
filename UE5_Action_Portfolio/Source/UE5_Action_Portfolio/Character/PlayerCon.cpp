#include "Character/PlayerCon.h"
#include "Global/Enums.h"

APlayerCon::APlayerCon()
{
    int Team = static_cast<int>(TeamType::PlayerTeam);

    TeamId = FGenericTeamId(Team);
}

FGenericTeamId APlayerCon::GetGenericTeamId() const
{
    return TeamId.GetId();
}
