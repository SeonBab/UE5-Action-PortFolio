#include "Character/PlayerCon.h"

APlayerCon::APlayerCon()
{
    TeamId = FGenericTeamId(0);
}

FGenericTeamId APlayerCon::GetGenericTeamId() const
{
    return TeamId;
}
