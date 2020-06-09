#include "bandatac.h"
#include "bandatalist.h"
using namespace BANLogic;
int BanDataList::datacount=1;

void BANLogic::BanDataList::printRPN()
{
    QTextStream(stdout)<<"[";
    foreach(BanDComponent *ptr, this->getDataList())
    {
        ptr->printRPN();
    }
    QTextStream(stdout)<<"]";
    cout<<endl;
}

void BANLogic::BanDataList::print()
{      

    foreach(QString val,this->getPrintStack())
    {
        QTextStream(stdout)<<val;
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

QList<BanSComponent *> BanDataList::getStList() const
{
    return StList;
}

BANLogic::BanDataList::BanDataList():BanSComponent(BanSComponentType::bData)
{
}

BANLogic::BanDataList::BanDataList(QList<BanDComponent *> dList):BanSComponent(BanSComponentType::bData)
{
    this->dataList=dList;
    this->instantiate=false;
    foreach(BanDComponent *ptr,dataList)
    {
        switch(ptr->getDtype())
        {
        case BanDComponentType::bAtom:
        {
            BanDAtom * batom = dynamic_cast<BanDAtom *>(ptr);
            BanDAtom *myatom= new BanDAtom(batom->getAtype(),batom->getID());
            instantiate=true;
            this->printStack.push(myatom->getID());
            break;
        }
        case BanDComponentType::bOperator:
        {
            instantiate=true;
            BanDOperator * bop = dynamic_cast<BanDOperator *>(ptr);
            BanDOperator *myOperator= new BanDOperator(bop->getDOtype());
            switch (myOperator->getDOtype())
            {
            case BanDOperatorType::concates:{
                this->printStack.push(this->printStack.pop()+","+this->printStack.pop());
                break;
            }
            case BanDOperatorType::Encryption:{
                this->printStack.push("{"+this->printStack.pop()+"}"+this->printStack.pop());
                break;
            }
            case BanDOperatorType::FreshData:{
                this->printStack.push(this->printStack.pop()+ptr->getID()+this->printStack.pop());
                //this->dataList.append(myOperator);
                break;
            }
            case BanDOperatorType::ShareKey:{
                this-> printStack.push(this->printStack.pop()+ptr->getID()+this->printStack.pop());
                break;
            }
            case BanDOperatorType::ShareSecret:{
                this-> printStack.push(this->printStack.pop()+ptr->getID()+this->printStack.pop());
                break;
            }
            case BanDOperatorType::HashKey:{
                this-> printStack.push(this->printStack.pop()+ptr->getID()+this->printStack.pop());
                break;
            }
            }
            break;
        }
        case BanDComponentType::bAnyData:
        {
            BanDataC * bdata = dynamic_cast<BanDataC *>(ptr);
            if(bdata->getMyListdata().size()<=1)
            {
                this->printStack.push(ptr->getID());
                this->instantiate=true;
                break;
            }
            else
            {
                BanDataList *mybn=new BanDataList(bdata->getMyListdata());
                foreach(QString val,mybn->getPrintStack())
                {
                    this-> printStack.push(val);
                    this->instantiate=true;
                }
            }
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
