#include "bandatac.h"
int BANLogic::BanDataC::dataCount=1;

QStack<QString> BANLogic::BanDataC::getPrintQStack() const
{
    return printQStack;
}

BANLogic::BanDataC::BanDataC():BanDComponent(BanDComponentType::bAnyData)
{
    if(this->dataValue=="" || this->dataValue.isEmpty() || this->dataValue.isNull())
    {
        this->dataValue=QString("_D_%1").arg(dataCount++);
        this->type=BanDComponentType::bAnyData;
        this->instantiate=false;
        myListdata.append(this);
        printQStack.push(this->getID());

    }
}

void BANLogic::BanDataC::setDtype(QString dType)
{
    if(dType=="bAnyData")
        type=BanDComponentType::bAnyData;
}

void BANLogic::BanDataC::setId(const QString &value)
{
    this->dataValue=value;
}

QString BANLogic::BanDataC::getID()
{
    return this->dataValue;
}


void BANLogic::BanDataC::print()
{
    foreach(BanDComponent *ptr,this->myListdata)
    {
        ptr->print();
        //QTextStream(stdout)<<ptr->getID();
    }
}

bool BANLogic::BanDataC::getInstantiate() const
{
    return instantiate;
}

bool BANLogic::BanDataC::match(BANLogic::BanDComponent *val)
{
    BanDComponentType ty1= this->getDtype();
    BanDComponentType ty2= val->getDtype();
    if(ty1==ty2)
    {
        if(ty1==BanDComponentType::bAnyData)
        {
            BanDataC *d1=dynamic_cast<BanDataC *>(this);
            BanDataC *d2=dynamic_cast<BanDataC *>(val);
            if(d1->getInstantiate()==true && d2->getInstantiate()==false)
            {
                ifMatches=true;
            }
            else if(d1->getInstantiate()==false && d2->getInstantiate()==true)
            {
                ifMatches=true;
            }
            else ifMatches=false;
        }
    }
    else if(ty1!=ty2)
    {
        if(ty1==BanDComponentType::bAnyData)
        {
            if(this->getInstantiate()==false)
            {
                switch(val->getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    if(val->getInstantiate()==true)
                        ifMatches=true;
                    break;
                }
                case BanDComponentType::bOperator:
                {
                    if(val->getInstantiate()==true)
                        ifMatches=false;
                    break;
                }
                case BanDComponentType::bAnyData:
                {
                    if(val->getInstantiate()==true)
                        ifMatches=true;
                    break;
                }
                }
            }
        }
        else if(ty2==BanDComponentType::bAnyData)
        {
            if(val->getInstantiate()==false)
            {
                switch(this->getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    if(this->getInstantiate()==true)
                        ifMatches=true;
                    break;
                }
                case BanDComponentType::bOperator:
                {
                    if(this->getInstantiate()==true)
                        ifMatches=false;
                    break;
                }
                case BanDComponentType::bAnyData:
                {
                    if(this->getInstantiate()==true)
                        ifMatches=true;
                    break;
                }
                }
            }
        }
    }
    return ifMatches;
}

bool BANLogic::BanDataC::unify(BANLogic::BanDComponent *value)
{
    BanDataC *comp1=dynamic_cast<BanDataC *>(this);

    if(value->getDtype()==BanDComponentType::bAtom)
    {
        if(comp1->getInstantiate()==false)
        {
            this->setId(comp1->getID());
            comp1->getMyListdata().append(value);
            unifies=true;
        }
    }
    return unifies;
}

bool BANLogic::BanDataC::getIfMatches() const
{
    return ifMatches;
}

QList<BANLogic::BanDComponent *> BANLogic::BanDataC::getMyListdata() const
{
    return myListdata;
}

void BANLogic::BanDataC::printRPN()
{
    foreach(BanDComponent *ptr, this->getMyListdata())
    {
        QTextStream(stdout)<<ptr->getID()<<" ";
    }
}
