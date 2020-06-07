#include "bansoperator.h"
using namespace BANLogic;
BanSOperatorType BANLogic::banSOperator::getStOptype() const
{
    return astype;
}


BANLogic::banSOperator::banSOperator()
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

void BANLogic::banSOperator::printRPN()
{

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
    return false;
}

bool BANLogic::banSOperator::unify(BanSComponent &value)
{
    return false;
}

bool BANLogic::banSOperator::getIfMatches() const
{
    return ifMatches;
}
