#include "bandatac.h"
#include "bandatalist.h"
using namespace BANLogic;
int BanDataList::datacount=1;

void BANLogic::BanDataList::printRPN()
{
    QTextStream(stdout)<<"[";

    foreach(BanDComponent *ptr,this->getDataList())
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
            BanDataC *d1=dynamic_cast<BanDataC *>(ptr);
            d1->printRPN();
            break;
        }
        }

        //QTextStream(stdout)<<ptr->getID();
    }
    QTextStream(stdout)<<"]";
    cout<<endl;

}

void BANLogic::BanDataList::print()
{
    if(this->getPrintStack().size()==1)
    {
//        foreach(QString val,this->getPrintStack())
//        {
            QTextStream(stdout)<<this->getPrintStack().top();
//        }
    }
    else
    {
        throw new BanException("Incorrect number of arguments passed, please check the parameters properly");
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
/*
QList<BanSComponent *> BanDataList::getStList() const
{
    return StList;
}
*/
BANLogic::BanDataList::BanDataList():BanSComponent(BanSComponentType::bData)
{
}

BANLogic::BanDataList::BanDataList(QList<BanDComponent *> dList):BanSComponent(BanSComponentType::bData)
{
    this->dataList=dList;
    //this->instantiate=true;
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
            //BanDataC * bdata1 = new BanDataC(BanDComponentType::bAnyData,bdata->getMyListdata());

            if(!bdata->getMyListdata().isEmpty())
            {
                this->printStack.push(ptr->getID());
                //this->instantiate=true;
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
                        BanDataC *d1=dynamic_cast<BanDataC *>(this->getDataList().value(i));
                        BanDataC *d2=dynamic_cast<BanDataC *>(data.getDataList().value(i));
                        if(d1->getDtype()==d2->getDtype())
                        {
                            if(d1->match(d2))
                                ifMatches=true;
                            else ifMatches=false;
                        }
                        break;
                    }
                    }
                }
            }
        }
        else if(this->getDataList().size()!=data.getDataList().size())
        {
            QList<BanDComponent*> mylist1=this->getDataList();
            QList<BanDComponent*> mylist2=data.getDataList();
            QList<BanDComponent*> temp;

            int i, mycount=0, juno=0,dataTypeIndex=0;
            for(i=0;i<mylist1.size();i++)
            {
                if(mylist2.value(juno)->getDtype()==mylist1.value(i)->getDtype())
                {
                    if(mylist2.value(juno)->getInstantiate()==true && mylist1.value(i)->getInstantiate()==false )
                    {
                        if(mylist1.value(i)->match(mylist2.value(juno)))
                        {
                            ifMatches=true;
                            mycount++;
                            juno++;
                        }
                        else
                        {
                            ifMatches=false;
                            break;
                        }
                    }
                    else if(mylist2.value(juno)->getDtype()==BanDComponentType::bOperator && mylist1.value(i)->getDtype()==BanDComponentType::bOperator )
                    {
                        if(mylist1.value(i)->match(mylist2.value(juno)))
                        {
                            ifMatches=true;
                            mycount++;
                            juno++;
                        }
                        else
                        {
                            ifMatches=false;
                            break;
                        }
                    }
                }
                else if(mylist2.value(i)->getDtype()!=mylist1.value(i)->getDtype())
                {
                    if(mylist1.value(i)->getDtype()==BanDComponentType::bAnyData)
                    {
                        //saving the index of AnyData
                        if(mylist1.at(i)->getDtype()==BanDComponentType::bAnyData)
                        {dataTypeIndex=i;}

                        for(int j=i; j<=mylist2.size()-(mylist1.size()-i);j++)
                        {
                            juno=j;

                            if(mylist1.value(i)->match(mylist2.value(j)))
                            {
                                ifMatches=true;
                                temp.append(mylist2.value(j));
                                mycount++;
                            }
                            else
                            {
                                ifMatches=false;
                                //break;
                            }

                            juno++;
                        }
                    }
                }
            }
            cout<<endl;
            if(ifMatches)
            {
                QTextStream(stdout)<<this->getDataList().at(dataTypeIndex)->getID()<< " =  ";
                foreach(BanDComponent *lo, temp)
                {
                    QTextStream(stdout) <<lo->getID()+"  "<<flush;
                }
                cout<<endl;
            }

        }
        break;
    }
    case BanSComponentType::bSOperator:
    {
        ifMatches=false;
        break;
    }
    case BanSComponentType::bStatement:
    {
        ifMatches=false;
        break;
    }
    }
    return ifMatches;
}

bool BANLogic::BanDataList::unify(BanSComponent &Scomp)
{
    if(this->match(Scomp))
    {
        QList<BanDComponent*> mylist1=this->getDataList();
        BanDataList &data=dynamic_cast<BanDataList&>(Scomp);
        QList<BanDComponent*> mylist2=data.getDataList();
        QList<BanDComponent*> temp;
        int l=mylist1.length();
        switch(Scomp.getStype())
        {
        case BanSComponentType::bData:
        {
            if(mylist1.size()==mylist2.size()){
                for(int i=0; i<l; i++)
                {
                    switch(mylist1.at(i)->getDtype())
                    {
                    case BanDComponentType::bAtom:
                    {
                        BanDAtom *atom1=dynamic_cast<BanDAtom*>(mylist1.value(i));
                        BanDAtom *atom2=dynamic_cast<BanDAtom*>(mylist2.value(i));
                        if(atom1->unify(atom2))
                        {
                            temp.append(data.getDataList().value(i));
                            // mylist1.replace(i,data.getDataList().value(i));
                            unifies=true;
                        }
                        else
                        {
                            unifies=false;
                            break;
                        }

                        break;
                    }
                    case BanDComponentType::bOperator:
                    {
                        BanDOperator *operator1=dynamic_cast<BanDOperator *>(mylist1.value(i));
                        BanDOperator *operator2=dynamic_cast<BanDOperator *>(mylist2.value(i));
                        if(operator1->unify(operator2))
                        {
                            temp.append(data.getDataList().value(i));
                            // mylist1.replace(i,data.getDataList().value(i));
                            unifies=true;
                        }
                        else
                        {
                            unifies=false;
                            break;
                        }
                        break;
                    }
                    case BanDComponentType::bAnyData:{
                        BanDataC *dataany1=dynamic_cast<BanDataC *>(mylist1.value(i));
                        if(dataany1->unify(mylist2.value(i)))
                        {
                            temp.append(data.getDataList().value(i));
                            //  mylist1.replace(i,data.getDataList().value(i));
                            unifies=true;
                        }
                        else
                        {
                            unifies=false;
                            break;
                        }
                        break;
                    }
                    }
                }
            }
            else if(mylist1.size()!=mylist2.size())
            {

                int i, mycount=0, juno=0;
                for(i=0;i<mylist1.size();i++)
                {
                    if(mylist2.value(juno)->getDtype()==mylist1.value(i)->getDtype())
                    {
                        if(mylist1.value(i)->match(mylist2.value(juno)))
                        {
                            if(mylist1.value(i)->unify(mylist2.value(juno)))
                            {
                                unifies=true;
                                temp.append(mylist2.value(juno));
                                mycount++;
                                juno++;
                            }
                            else
                            {
                                juno++;
                                unifies=false;
                                break;
                            }
                        }

                    }
                    else if(mylist2.value(i)->getDtype()!=mylist1.value(i)->getDtype())
                    {
                        if(mylist1.value(i)->getDtype()==BanDComponentType::bAnyData)
                        {
                            for(int j=i; j<=mylist2.size()-(mylist1.size()-i);j++)
                            {
                                juno=j;
                                if(mylist1.value(i)->unify(mylist2.value(j)))
                                {
                                    temp.append(mylist2.value(j));
                                    unifies=true;
                                    mycount++;
                                }

                                juno++;
                            }
                        }
                    }
                }
                if(unifies)
                {
                    BanDataList *finalList=new BanDataList(temp);
                    this->dataList=temp;
                    this->printStack=finalList->printStack;
                    data.dataList=this->dataList;
                    data.printStack=this->printStack;
                }
            }
            break;
        }
        case BanSComponentType::bSOperator:
        {
            unifies=false;
            break;
        }
        case BanSComponentType::bStatement:
        {
            unifies=false;
            break;
        }
        }
    }
    return unifies;
}

bool BanDataList::getInstantiate() const
{
    return instantiate;
}

bool BanDataList::getIfMatches() const
{
    return ifMatches;
}
