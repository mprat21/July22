#include "bandoperator.h"
using namespace BANLogic;

BanDOperatorType BANLogic::BanDOperator::getDOtype() const
{
    return adtype;
}

void BANLogic::BanDOperator::setDtype(QString oVal)
{
    if(oVal=="concates")
    {
        adtype=BanDOperatorType::concates;
    }
    else if(oVal=="encryptedby")
    {
        adtype=BanDOperatorType::Encryption;
    }
    else if(oVal=="fresh")
    {
        adtype=BanDOperatorType::FreshData;
    }
    else if(oVal=="shared Key")
    {
        adtype=BanDOperatorType::ShareKey;
    }
    else if(oVal=="share secret")
    {
        adtype=BanDOperatorType::ShareSecret;
    }
    else if(oVal=="Hash")
    {
        adtype=BanDOperatorType::HashKey;
    }
}

bool BANLogic::BanDOperator::match(BanDComponent *value)
{
    switch(value->getDtype())
    {
    case BanDComponentType::bAtom:
    {
        this->ifMatches=false;
        break;
    }
    case BanDComponentType::bOperator:
    {
        BanDOperator *bop = dynamic_cast<BanDOperator *>(value);
        if(this->getDOtype()==bop->getDOtype())
        {
            this->ifMatches=true;
        }else this->ifMatches=false;
        break;
    }
    case BanDComponentType::bAnyData:
    {
        this->ifMatches=false;
        break;
    }
    default:
    {
        throw new BanException("Unrecognised Component Type in banDOperator::match()");
    }
    }


    return this->ifMatches;
}

bool BANLogic::BanDOperator::unify(BanDComponent *value)
{
    if(this->match(value))
    {
        switch(value->getDtype())
        {
        case BanDComponentType::bAtom:
        {
            this->unifies=false;
            break;
        }
        case BanDComponentType::bOperator:
        {
            BanDOperator *bop = dynamic_cast<BanDOperator *>(value);
            if(this->getDOtype()==bop->getDOtype())
            {
                this->unifies=true;
                this->setId(bop->getID());
            }
            else
                this->unifies=false;
            break;
        }
        case BanDComponentType::bAnyData:
        {
            this->unifies=false;
            break;
        }
        default:
        {
            throw new BanException("Unrecognised Component Type in banDOperator::unify()");
        }
        }
    }
    return this->unifies;
}

void BANLogic::BanDOperator::setAdtype(const BanDOperatorType &value)
{
    adtype = value;
}

bool BANLogic::BanDOperator::getInstantiate() const
{
    return instantiate;
}

void BANLogic::BanDOperator::setId(const QString &value)
{
    oValue = value;
}

bool BanDOperator::getIfMatches() const
{
    return ifMatches;
}

BANLogic::BanDOperator::BanDOperator():BanDComponent(BanDComponentType::bOperator)
{

}

BANLogic::BanDOperator::BanDOperator(BanDOperatorType adTy):BanDComponent(BanDComponentType::bOperator), adtype(adTy)
{
    switch(adTy)
    {
    case BanDOperatorType::concates:{
        oValue=" concates ";
        this->instantiate=true;
        break;
    }
    case BanDOperatorType::Encryption:{
        oValue="encryptedby";
        this->instantiate=true;
        break;
    }
    case BanDOperatorType::FreshData:{
        oValue="fresh";
        this->instantiate=true;
        break;
    }
    case BanDOperatorType::ShareKey:{
        oValue="shared Key";
        this->instantiate=true;
        break;
    }
    case BanDOperatorType::ShareSecret:{
        oValue="share secret";
        this->instantiate=true;
        break;
    }
    case BanDOperatorType::HashKey:{
        oValue="Hash";
        this->instantiate=true;
        break;
    }
    default:
    {
        oValue="";
        instantiate=false;
    }
    }

}
