#include "bandatac.h"
namespace BANLogic {

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
        printQStack.push(this->getID());
    }
}

BanDataC::BanDataC(BanDataC &orig):BanDComponent(BanDComponentType::bAnyData)
{
    setId(orig.getID());
    type = orig.type;
    dList=orig.dList;
    myListdata=orig.myListdata;
    printQStack=orig.printQStack;
    instantiate=orig.instantiate;
    //myListdata.append(this);
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

bool BANLogic::BanDataC::match(BANLogic::BanDComponent &val)
{
    BanDComponentType ty1= this->getDtype();
    BanDComponentType ty2= val.getDtype();
    if(ty1==ty2)
    {
        if(ty1==BanDComponentType::bAnyData)
        {
            BanDataC *d1=dynamic_cast<BanDataC *>(this);
            BanDataC &d2=dynamic_cast<BanDataC&>(val);
            if(d1->getInstantiate()==true && d2.getInstantiate()==false)
            {
                ifMatches=true;
            }
            else if(d1->getInstantiate()==false && d2.getInstantiate()==true)
            {
                ifMatches=true;
            }
            else if(d1->getInstantiate()==false && d2.getInstantiate()==false)
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
                switch(val.getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    if(val.getInstantiate()==true)
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
                    if(val.getInstantiate()==true)
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
            if(val.getInstantiate()==false)
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
void BanDataC::instantiateObject(BanDComponent *value)
{
    switch (value->getDtype()) {
    case BanDComponentType::bAtom:
    {
        BanDAtom *atm= dynamic_cast<BanDAtom*>(value);
        this->myListdata.append(atm);
        this->printQStack.push(atm->getID());
        break;
    }
    case BanDComponentType::bOperator:
    {
        BanDOperator *op= dynamic_cast<BanDOperator*>(value);
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
            this->printQStack.push(this->printQStack.pop()+op->getID()+this->printQStack.pop());
            this->myListdata.append(op);
            break;
        }
        case BanDOperatorType::ShareKey:{
            this-> printQStack.push(this->printQStack.pop()+op->getID()+this->printQStack.pop());
            this->myListdata.append(op);
            break;
        }
        case BanDOperatorType::ShareSecret:{
            this-> printQStack.push(this->printQStack.pop()+op->getID()+this->printQStack.pop());
            this->myListdata.append(op);
            break;
        }
        case BanDOperatorType::HasKey:{
            this-> printQStack.push(this->printQStack.pop()+op->getID()+this->printQStack.pop());
            this->myListdata.append(op);
            break;
        }
        default:
        {
            throw new BanException("Unrecognised Component Type in banDataC::unify()");
        }
        }
        break;
    }
    case BanDComponentType::bAnyData:
    {
        //BanDataC *atm= new BanDataC();
        BanDataC *dc= dynamic_cast<BanDataC*>(value);
        //this->myListdata.append(dc);
        //this->printQStack.push(dc->getID());
        break;
    }
    }

}
bool BANLogic::BanDataC::unify(BANLogic::BanDComponent &value)
{
    if(this->match(value))
    {
        switch(value.getDtype())
        {
        case BanDComponentType::bAtom:
        {
            BanDAtom &comp2=dynamic_cast<BanDAtom&>(value);
            if(comp2.getInstantiate()==true)
            {
                this->setId(comp2.getID());
                unifies=true;
            }
            else unifies=false;
            break;
        }
        case BanDComponentType::bOperator:
        {
            BanDOperator &comp2=dynamic_cast<BanDOperator&>(value);
            if(comp2.getInstantiate()==true)
            {
                unifies=false;
            }
            break;

        }
        case BanDComponentType::bAnyData:
        {
            // QTextStream(stdout) <<this->getID()+ " " <<flush;

            BanDataC &comp2=dynamic_cast<BanDataC&>(value);
            if(comp2.getInstantiate()==true)
            {
                this->setId(comp2.getID());
                unifies=true;
            }
            else
                unifies=false;
            break;

        }

        }
    }
    if(unifies)
    {
        if(value.getDtype()!=BanDComponentType::bOperator)
            QTextStream(stdout) <<value.getID()+ " " <<flush;
        //this->setId(this->printQStack.top());

        // this->instantiate=false;
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
    foreach(BanDComponent *ptr,this->myListdata)
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

QString BANLogic::BanDataC::getString()
{
    QString s = "";
//        foreach(QString ptr,this->printQStack)
//        {
//            s.append(ptr);
//        }
  // this->setId(s);

    foreach(BanDComponent *ptr,this->myListdata)
    {
        switch(ptr->getDtype())
        {
        case BanDComponentType::bAtom:
        {
            BanDAtom *d1=dynamic_cast<BanDAtom *>(ptr);
            s.append((d1->getString()));
            break;
        }
        case BanDComponentType::bOperator:
        {
            BanDOperator *d1=dynamic_cast<BanDOperator *>(ptr);
            s.append(d1->getString());
            break;
        }
        case BanDComponentType::bAnyData:
        {
            s.append(ptr->getID());
            //ptr->getString();
            break;
        }
        default:
        {
            throw new BanException("Unrecognised Component Type in banDataC::getString()");
        }
        }
    }

    return s;
}

bool BANLogic::BanDataC::operator ==(const BANLogic::BanDComponent &other)
{
    bool equals = true;
    const BanDataC &d = dynamic_cast<const BanDataC &>(other);
    if (type != d.type)
    {
        if(type==BanDComponentType::bAnyData)
        {
            if(this->getInstantiate()==false)
            {
                switch(d.getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    if(d.getInstantiate()==true)
                        equals=true;
                    else equals=false;
                    break;
                }
                case BanDComponentType::bOperator:
                {
                    // if(val->getInstantiate()==true)
                    equals=false;
                    break;
                }
                case BanDComponentType::bAnyData:
                {
                    if(d.getInstantiate()==true)
                        equals=true;
                    else equals=false;
                    break;
                }
                default:
                {
                    throw new BanException("Unrecognised Component Type in banDataC::operator ==()");
                }
                }
            }
            equals = false;
        }
        else if (d.type == BanDComponentType::bAnyData)
        {
            if(d.getInstantiate()==false)
            {
                switch(this->getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    if(this->getInstantiate()==true)
                        equals=true;
                    else equals=false;
                    break;
                }
                case BanDComponentType::bOperator:
                {
                    //if(this->getInstantiate()==true)
                    equals=false;
                    break;
                }
                case BanDComponentType::bAnyData:
                {
                    if(this->getInstantiate()==true)
                        equals=true;
                    else equals=false;
                    break;
                }
                default:
                {
                    throw new BanException("Unrecognised Component Type in banDataC::operator ==()");
                }
                }
            }
        }
    }
    return equals;
}


}
