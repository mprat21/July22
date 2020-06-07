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
    BanDOperator * dop = dynamic_cast<BanDOperator *>(value);
    if(this->getID().toStdString()==(dop->getID().toStdString()))
        return true;
    else
        return false;
}

bool BANLogic::BanDOperator::unify(BanDComponent *value)
{
    if(this->match(value))
    {
        switch(value->getDtype())
        {
        case BanDComponentType::bAtom:
        {
            break;
        }
        case BanDComponentType::bOperator:
        {
            BanDOperator *bop = dynamic_cast<BanDOperator *>(value);
            if(this->getID()!=value->getID())
            {
                if(this->getInstantiate()==true && bop->getInstantiate()==false)
                {
                    QTextStream(stdout) <<this->getID();
                }
                else if(bop->getInstantiate()==true && this->getInstantiate()==false)
                {
                    QTextStream(stdout) <<bop->getID();
                }
            }
        }
        }
    }
    return false;
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

BANLogic::BanDOperator::BanDOperator()
{

}

BANLogic::BanDOperator::BanDOperator(BanDOperatorType adTy):BanDComponent(BanDComponentType::bOperator), adtype(adTy)
{
    switch(adTy)
    {
    case BanDOperatorType::concates:{
        oValue="concates ";
        break;
    }
    case BanDOperatorType::Encryption:{
        oValue="encryptedby";
        break;
    }
    case BanDOperatorType::FreshData:{
        oValue="fresh";
        break;
    }
    case BanDOperatorType::ShareKey:{
        oValue="shared Key";
        break;
    }
    case BanDOperatorType::ShareSecret:{
        oValue="share secret";
        break;
    }
    case BanDOperatorType::HashKey:{
        oValue="Hash";
        break;
    }
    default:
    {
        oValue="";
        instantiate=false;
    }
        instantiate=true;
    }

}
