#include "bansoperator.h"
using namespace BANLogic;
BanSOperatorType BANLogic::banSOperator::getStOptype() const
{
    return astype;
}


BANLogic::banSOperator::banSOperator():BanSComponent(BanSComponentType::bSOperator)
{

}

BANLogic::banSOperator::banSOperator(BanSOperatorType adTy):BanSComponent(BanSComponentType::bSOperator),astype(adTy)
{
    switch(adTy)
    {
    case BanSOperatorType::told:{
        soValue=" told ";
        break;
    }
    case BanSOperatorType::believe:{
        soValue=" believes ";
        break;
    }
    case BanSOperatorType::possess:{
        soValue=" possess ";
        break;
    }
    case BanSOperatorType::conveyed:{
        soValue=" conveyed ";
        break;
    }
    case BanSOperatorType::hasJurisdiction:{
        soValue=" hasJurisdiction over ";
        break;
    }
    }
}


BANLogic::banSOperator::~banSOperator()
{

}

bool BANLogic::banSOperator::getInstantiate() const
{
    return instantiate;
}

bool BANLogic::banSOperator::match(BanSComponent &value)
{
    return ifMatches;
}

bool BANLogic::banSOperator::unify(BanSComponent &value)
{
    return unifies;
}

bool BANLogic::banSOperator::getIfMatches() const
{
    return ifMatches;
}
