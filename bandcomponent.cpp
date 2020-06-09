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

void BanDComponent::setInstantiate(bool value)
{
    instantiate = value;
}

BANLogic::BanDComponent::BanDComponent()
{
}

BANLogic::BanDComponent::BanDComponent(BanDComponentType dcTy):dtype(dcTy)
{
}
