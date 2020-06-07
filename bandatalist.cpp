#include "bandatalist.h"
using namespace BANLogic;
int BanDataList::datacount=1;

void BANLogic::BanDataList::printRPN()
{
    QTextStream(stdout)<<"[";
    foreach(BanDComponent *ptr, this->getDataList())
    {
        ptr->print();
    }
    QTextStream(stdout)<<"]";
    cout<<endl;
}

void BANLogic::BanDataList::print()
{
    BanDataList *temp=new BanDataList(this->getDataList());
    foreach(QString ptr, temp->getPrintStack())
    {
        QTextStream(stdout)<<ptr;
    }
}

QList<BanDComponent *> BANLogic::BanDataList::getDataList()
{
    return dataList;
}

QStack<QString> BANLogic::BanDataList::getPrintStack()
{
    return printStack;
}

BANLogic::BanDataList::BanDataList()
{
    if(this->dataID=="" || this->dataID.isEmpty() || this->dataID.isNull())
    {
        this->dataID=QString("_D_%1").arg(datacount++);
        this->instantiate=false;
        QTextStream(stdout)<<this->dataID<<endl;
        StList.append(this);
    }
    foreach(BanDComponent *ptr, this->getDataList())
    {
        switch(ptr->getDtype())
        {
        case BanDComponentType::bAtom:
        {
            instantiate=true;
            printStack.push(ptr->getID());
            break;
        }
        case BanDComponentType::bOperator:
        {
            instantiate=true;
            BanDOperator * bop = dynamic_cast<BanDOperator *>(ptr);
            switch (bop->getDOtype())
            {
            case BanDOperatorType::concates:{
                printStack.push(printStack.pop()+","+printStack.pop());
                break;
            }
            case BanDOperatorType::Encryption:{
                printStack.push("{"+printStack.pop()+"}"+printStack.pop());
                break;
            }
            case BanDOperatorType::FreshData:{
                printStack.push(printStack.pop()+ptr->getID()+printStack.pop());
                break;
            }
            case BanDOperatorType::ShareKey:{
                printStack.push(printStack.pop()+ptr->getID()+printStack.pop());
                break;
            }
            case BanDOperatorType::ShareSecret:{
                printStack.push(printStack.pop()+ptr->getID()+printStack.pop());
                break;
            }
            case BanDOperatorType::HashKey:{
                printStack.push(printStack.pop()+ptr->getID()+printStack.pop());
                break;
            }
            }
            break;
        }
        case BanDComponentType::bAnyData:
        {
            if(instantiate==true)
            {
                BanDataList *dL = dynamic_cast<BanDataList *>(ptr);
                BanDataList *dL2=new BanDataList(dL->getDataList());
                instantiate=true;
            }
            break;
        }
        }
    }
}

BANLogic::BanDataList::BanDataList(QList<BanDComponent *> dList):BanSComponent(BanSComponentType::bData)
{
    dataList=dList;
    this->instantiate=false;
    foreach(BanDComponent *ptr, this->getDataList())
    {
        switch(ptr->getDtype())
        {
        case BanDComponentType::bAtom:
        {
            instantiate=true;
            printStack.push(ptr->getID());
            break;
        }
        case BanDComponentType::bOperator:
        {
            instantiate=true;
            BanDOperator * bop = dynamic_cast<BanDOperator *>(ptr);
            switch (bop->getDOtype())
            {
            case BanDOperatorType::concates:{
                printStack.push(printStack.pop()+","+printStack.pop());
                break;
            }
            case BanDOperatorType::Encryption:{
                printStack.push("{"+printStack.pop()+"}"+printStack.pop());
                break;
            }
            case BanDOperatorType::FreshData:{
                printStack.push(printStack.pop()+ptr->getID()+printStack.pop());
                break;
            }
            case BanDOperatorType::ShareKey:{
                printStack.push(printStack.pop()+ptr->getID()+printStack.pop());
                break;
            }
            case BanDOperatorType::ShareSecret:{
                printStack.push(printStack.pop()+ptr->getID()+printStack.pop());
                break;
            }
            case BanDOperatorType::HashKey:{
                printStack.push(printStack.pop()+ptr->getID()+printStack.pop());
                break;
            }
            }
            break;
        }
        case BanDComponentType::bAnyData:
        {
            if(this->instantiate==true)
            {
                BanDataList *dL = dynamic_cast<BanDataList *>(ptr);
                BanDataList *dL2=new BanDataList(dL->getDataList());
                instantiate=true;
            }
            break;
        }
        }
    }
}

BanDataList::BanDataList(QList<BanSComponent *> dList)
{
    foreach(BanSComponent *ptr, dList)
    {
        switch(ptr->getStype())
        {
        case BanSComponentType::bData:
        {
            BanDataList *dL = dynamic_cast<BanDataList *>(ptr);

            if(dL->dataID=="" || dL->dataID.isEmpty() || dL->dataID.isNull())
            {
                dL->instantiate=false;
                QTextStream(stdout)<<dL->datacount<<endl;
                QTextStream(stdout)<<dL->dataID<<endl;
            }
            else
            {
                BanDataList *dL = dynamic_cast<BanDataList *>(ptr);
                BanDataList *dL2=new BanDataList(dL->getDataList());
                this->instantiate=true;
                //printStack.push(ptr->getID());
            }
            break;
        }
        case BanSComponentType::bSOperator:
        {
            instantiate=true;
            BanDOperator * bop = dynamic_cast<BanDOperator *>(ptr);
            switch (bop->getDOtype())
            {
            case BanDOperatorType::concates:{
                printStack.push(printStack.pop()+","+printStack.pop());
                break;
            }
            case BanDOperatorType::Encryption:{
                printStack.push("{"+printStack.pop()+"}"+printStack.pop());
                break;
            }
            case BanDOperatorType::FreshData:{
                printStack.push(printStack.pop()+ptr->getID()+printStack.pop());
                break;
            }
            case BanDOperatorType::ShareKey:{
                printStack.push(printStack.pop()+ptr->getID()+printStack.pop());
                break;
            }
            case BanDOperatorType::ShareSecret:{
                printStack.push(printStack.pop()+ptr->getID()+printStack.pop());
                break;
            }
            case BanDOperatorType::HashKey:{
                printStack.push(printStack.pop()+ptr->getID()+printStack.pop());
                break;
            }
            }
            break;
        }
        case BanSComponentType::bStatement:
        {

        }
        }
    }

}

bool BANLogic::BanDataList::match(BanSComponent &Scomp)
{
    int count=0;
    switch(Scomp.getStype())
    {
    case BanSComponentType::bData:
    {
        BanDataList &data=dynamic_cast<BanDataList&>(Scomp);
        if(this->getDataList().size()==data.getDataList().size())
        {
            int length=this->getDataList().size();
            for(int i=0; i<length; i++)
            {
                if(this->getDataList().at(i)->getDtype()==data.getDataList().at(i)->getDtype())
                {
                    switch(this->getDataList().at(i)->getDtype())
                    {
                    case BanDComponentType::bAtom:
                    {
                        BanDAtom *atom1=dynamic_cast<BanDAtom *>(this->getDataList().value(i));
                        BanDAtom *atom2=dynamic_cast<BanDAtom *>(data.getDataList().value(i));
                        if(atom1->match(atom2))
                            ifMatches=true;
                        else ifMatches=false;
                        break;
                    }
                    case BanDComponentType::bOperator:
                    {
                        BanDOperator *operator1=dynamic_cast<BanDOperator *>(this->getDataList().value(i));
                        BanDOperator *operator2=dynamic_cast<BanDOperator *>(data.getDataList().value(i));
                        if(operator1->getDOtype()==operator2->getDOtype())
                        {
                            if(operator1->match(operator2))
                                ifMatches=true;
                            else ifMatches=false;
                        }
                        break;
                    }
                    case BanDComponentType::bAnyData:
                    {
                        BanDataList *dL= new BanDataList(this->getDataList());
                        QTextStream(stdout)<<dL->getID();
                        break;
                    }
                    }
                }
                else
                {
                    if(this->getInstantiate()==false)
                    {
                        foreach(BanDComponent *ptr, data.getDataList())
                        {
                            BanDataList *d1=dynamic_cast<BanDataList *>(ptr);
                            //d1->getPrintStack().pop();

                            //this->dataID=ptr->

                        }
                    }
                    //else if(data.getInstantiate()==false)
                }
            }
        }
        break;
    }
    }
    return ifMatches;
}

bool BANLogic::BanDataList::unify(BanSComponent &Scomp)
{
    int l=this->getDataList().length();
    if(this->match(Scomp))
    {
        switch(Scomp.getStype())
        {
        case BanSComponentType::bData:
        {
            BanDataList &data=dynamic_cast<BanDataList&>(Scomp);
            for(int i=0; i<l; i++)
            {
                switch(this->getDataList().at(i)->getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    BanDAtom *atom1=dynamic_cast<BanDAtom*>(this->getDataList().value(i));
                    BanDAtom *atom2=dynamic_cast<BanDAtom*>(data.getDataList().value(i));
                    bool unifies=atom1->unify(atom2);
                    if(unifies)
                    {
                        this->getDataList().replace(i,data.getDataList().value(i));
                    }
                    break;
                }
                case BanDComponentType::bOperator:
                {
                    BanDOperator *operator1=dynamic_cast<BanDOperator *>(this->getDataList().value(i));
                    BanDOperator *operator2=dynamic_cast<BanDOperator *>(data.getDataList().value(i));
                    bool unifies=operator1->unify(operator2);
                    if(unifies)
                    {
                        this->getDataList().replace(i,data.getDataList().value(i));
                    }
                    break;
                }
                }
            }
            return true;
            break;
        }
        case BanSComponentType::bSOperator:
        {
            // return this->unify(Scomp);
            break;
        }
        case BanSComponentType::bStatement:
        {
            break;
        }
        }
    }
    return false;
}

bool BanDataList::getInstantiate() const
{
    return instantiate;
}

bool BanDataList::getIfMatches() const
{
    return ifMatches;

}
