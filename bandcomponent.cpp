#include "bandcomponent.h"
using namespace BANLogic;
BanDComponentType BANLogic::BanDComponent::getDtype() const
{
    return dtype;
}

bool BanDComponent::getIfMatches() const
{
    return ifMatches;
}

BANLogic::BanDComponent::BanDComponent()
{
}

BANLogic::BanDComponent::BanDComponent(BanDComponentType dcTy):dtype(dcTy)
{
}
