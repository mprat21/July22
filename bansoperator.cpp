#include "bansoperator.h"
using namespace BANLogic;
BanSOperatorType BANLogic::BanSOperator::getStOptype() const
{
    return astype;
}


BANLogic::BanSOperator::BanSOperator():BanSComponent(BanSComponentType::bSOperator)
{

}

BANLogic::BanSOperator::BanSOperator(BanSOperatorType adTy):BanSComponent(BanSComponentType::bSOperator),astype(adTy)
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


BANLogic::BanSOperator::~BanSOperator()
{

}

bool BANLogic::BanSOperator::getInstantiate() const
{
    return instantiate;
}

bool BANLogic::BanSOperator::match(BanSComponent &value)
{
    switch(value.getStype())
    {
    case BanSComponentType::bData:
    {
        this->ifMatches=false;
        break;
    }
    case BanSComponentType::bSOperator:
    {
        BanSOperator bop = dynamic_cast<BanSOperator&>(value);
        if(this->getStOptype()==bop.getStOptype())
        {
            this->ifMatches=true;
        }else this->ifMatches=false;
        break;
    }
    case BanSComponentType::bStatement:
    {
        this->ifMatches=false;
        break;
    }
    default:
    {
        throw new BanException("Unrecognised Component Type in banSOperator::match()");
    }
    }
    return this->ifMatches;
}

bool BANLogic::BanSOperator::unify(BanSComponent &value)
{
    if(this->match(value))
    {
        switch(value.getStype())
        {
        case BanSComponentType::bData:
        {
            this->unifies=false;
            break;
        }
        case BanSComponentType::bSOperator:
        {
            BanSOperator &bop = dynamic_cast<BanSOperator&>(value);
            if(this->getStOptype()==bop.getStOptype())
            {
                this->unifies=true;
                this->setId(bop.getID());
            }
            else
                this->unifies=false;
            break;
        }
        case BanSComponentType::bStatement:
        {
            this->unifies=false;
            break;
        }
        default:
        {
            throw new BanException("Unrecognised Component Type in banSOperator::unify()");
        }
        }
    }
    return this->unifies;
}

bool BANLogic::BanSOperator::getIfMatches() const
{
    return ifMatches;
}


void BANLogic::BanSOperator::setId(const QString &value)
{
    soValue=value;
}
