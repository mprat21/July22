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
        this->dataValue=QString("_X%1_").arg(dataCount++);
        this->type=BanDComponentType::bAnyData;
        this->instantiate=false;
        myListdata.append(this);
        //printQStack.push(this->getID());
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
    foreach(QString ptr,this->printQStack)
    {
        QTextStream(stdout)<<ptr;
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
            else if(d1->getInstantiate()==false && d2->getInstantiate()==false)
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
                    else ifMatches=false;
                    break;
                }
                case BanDComponentType::bOperator:
                {
                   // if(val->getInstantiate()==true)
                        ifMatches=false;
                    break;
                }
                case BanDComponentType::bAnyData:
                {
                    if(val->getInstantiate()==true)
                        ifMatches=true;
                    else ifMatches=false;
                    break;
                }
                default:
                {
                    throw new BanException("Unrecognised Component Type in banDataC::match()");
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
                    else ifMatches=false;
                    break;
                }
                case BanDComponentType::bOperator:
                {
                    //if(this->getInstantiate()==true)
                        ifMatches=false;
                    break;
                }
                case BanDComponentType::bAnyData:
                {
                    if(this->getInstantiate()==true)
                        ifMatches=true;
                    else ifMatches=false;
                    break;
                }
                default:
                {
                    throw new BanException("Unrecognised Component Type in banDataC::match()");
                }
                }
            }
        }
    }
   // this->dataValue=printQStack.top();
    return ifMatches;
}

bool BANLogic::BanDataC::unify(BANLogic::BanDComponent *value)
{
    if(this->match(value))
    {
        if(value->getDtype()==BanDComponentType::bAtom)
        {
            BanDAtom *comp2=dynamic_cast<BanDAtom *>(value);
            if(comp2->getInstantiate()==true)
            {
                BanDAtom *atm= new BanDAtom(comp2->getAtype(),comp2->getID());
                this->myListdata.append(atm);
                this->printQStack.push(atm->getID());
                unifies=true;
            }
            else unifies=false;
        }
        else if(value->getDtype()==BanDComponentType::bOperator)
        {
            BanDOperator *comp2=dynamic_cast<BanDOperator *>(value);
            if(comp2->getInstantiate()==true)
            {
                BanDOperator *op= new BanDOperator(comp2->getDOtype());
                switch (op->getDOtype())
                {
                case BanDOperatorType::concates:{
                    this->printQStack.push(this->printQStack.pop()+","+this->printQStack.pop());
                     this->myListdata.append(op);
                    break;
                }
                case BanDOperatorType::Encryption:{
                    this->printQStack.push("{"+this->printQStack.pop()+"}"+this->printQStack.pop());
                     this->myListdata.append(op);

                    break;
                }
                case BanDOperatorType::FreshData:{
                    this->printQStack.push(this->printQStack.pop()+value->getID()+this->printQStack.pop());
                    this->myListdata.append(op);
                    break;
                }
                case BanDOperatorType::ShareKey:{
                    this-> printQStack.push(this->printQStack.pop()+value->getID()+this->printQStack.pop());
                    this->myListdata.append(op);
                    break;
                }
                case BanDOperatorType::ShareSecret:{
                    this-> printQStack.push(this->printQStack.pop()+value->getID()+this->printQStack.pop());
                    break;
                }
                case BanDOperatorType::HasKey:{
                    this-> printQStack.push(this->printQStack.pop()+value->getID()+this->printQStack.pop());
                    this->myListdata.append(op);
                    break;
                }
                default:
                {
                    throw new BanException("Unrecognised Component Type in banDataC::unify()");
                }
                }
                unifies=false;
            }
        }
        else if(value->getDtype()==BanDComponentType::bAnyData)
        {
           // QTextStream(stdout) <<this->getID()+ " " <<flush;

            BanDataC *comp2=dynamic_cast<BanDataC *>(this);
            if(comp2->getInstantiate()==true)
            {
                BanDataC *atm= new BanDataC();
                //this->getMyListdata().append(value);
                unifies=true;
            }
            else
                 unifies=false;
            //this->getMyListdata().append(value);

        }
    }
    if(unifies)
    {
        if(value->getDtype()!=BanDComponentType::bOperator)
            QTextStream(stdout) <<value->getID()+ " " <<flush;

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
    foreach(BanDComponent *ptr,this->getMyListdata())
    {
        switch(ptr->getDtype())
        {
        case BanDComponentType::bAtom:
        {
            BanDAtom *d1=dynamic_cast<BanDAtom *>(ptr);
            d1->printRPN();
            break;
        }
        case BanDComponentType::bOperator:
        {
            BanDOperator *d1=dynamic_cast<BanDOperator *>(ptr);
            d1->printRPN();
            break;
        }
        case BanDComponentType::bAnyData:
        {
            QTextStream(stdout) <<ptr->getID()+" "<<flush;
            break;
        }
        default:
        {
            throw new BanException("Unrecognised Component Type in banDataC::printRPN()");
        }
        }
        //QTextStream(stdout)<<ptr->getID();
    }
}
