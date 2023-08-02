#include "Character/PlayerCon.h"

FGenericTeamId APlayerCon::GetGenericTeamId() const
{
    return uint8(TeamId);
}
