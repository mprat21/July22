#include "bansoperator.h"
using namespace BANLogic;
namespace BANLogic {

BanSOperatorType BANLogic::BanSOperator::getStOptype() const
{
    return astype;
}

BANLogic::BanSOperator::BanSOperator(BanSOperator &orig):BanSComponent(BanSComponentType::bSOperator)
{
    soValue=orig.soValue;
    astype=orig.astype;
    stype=orig.stype;
    instantiate=orig.instantiate;
}

BANLogic::BanSOperator::BanSOperator():BanSComponent(BanSComponentType::bSOperator)
{

}

BANLogic::BanSOperator::BanSOperator(BanSOperatorType adTy):BanSComponent(BanSComponentType::bSOperator),astype(adTy)
{
    switch(adTy)
    {
    case BanSOperatorType::said:{
        soValue=" once said ";
        break;
    }
    case BanSOperatorType::believes:{
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
    case BanSOperatorType::controls:{
        soValue=" hasJurisdiction over ";
        break;
    }
    case BanSOperatorType::sees:{
        soValue=" sees ";
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

QString BANLogic::BanSOperator::getString()
{
    return soValue;
}

bool BANLogic::BanSOperator::operator ==(const BanSComponent &other)
{
    bool equals = true;
    const BanSOperator &o = dynamic_cast<const BanSOperator &>(other);
    if (astype != o.astype) {
        equals = false;
    } else {
         if(soValue == o.soValue) equals = true;
         else equals = false;
    }
    return equals;
}
}
