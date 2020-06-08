#include "banscomponent.h"
using namespace BANLogic;
BanSComponentType BANLogic::BanSComponent::getStype() const
{
    return stype;
}

BANLogic::BanSComponent::BanSComponent()
{
}

BANLogic::BanSComponent::BanSComponent(BanSComponentType sTy):stype(sTy)
{
}

BanSComponent::BanSComponent(QList<BanSComponent *> sList)
{


}

