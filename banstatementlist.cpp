#include "banstatementlist.h"
using namespace BANLogic;
int BANLogic::BanStatementList::statementCount=1;

QList<BanSComponent *> BanStatementList::getStList() const
{
    return stList;
}

QStack<QString> BanStatementList::getPrintStStack() const
{
    return printStStack;
}

BANLogic::BanStatementList::BanStatementList():BanSComponent(BanSComponentType::bStatement)
{
    if(this->stid=="" || this->stid.isEmpty() || this->stid.isNull())
    {
        this->stid=QString("_St_%1").arg(statementCount++);
        this->instantiate=false;
        stList.append(this);
        printStStack.push(this->stid);
    }
}
BANLogic::BanStatementList::BanStatementList(QList<BanSComponent *> sList):BanSComponent(BanSComponentType::bStatement)
{
    this->instantiate=true;

    this->stList=sList;
    foreach(BanSComponent *ptr, this->stList)
    {
        switch(ptr->getStype())
        {
        case BanSComponentType::bData:
        {
            BanDataList *comp1=dynamic_cast<BanDataList *>(ptr);
            foreach(QString ptr, comp1->getPrintStack())
            {
                this->printStStack.push(ptr);
            }
            break;
        }
        case BanSComponentType::bSOperator:
        {
            BanSOperator *comp1=dynamic_cast<BanSOperator *>(ptr);
            switch (comp1->getStOptype())
            {
            case BanSOperatorType::told:{
                this->printStStack.push(this->printStStack.pop()+comp1->getID()+this->printStStack.pop());
                break;
            }
            case BanSOperatorType::believe:{
                this->printStStack.push(this->printStStack.pop()+comp1->getID()+this->printStStack.pop());
                break;
            }
            case BanSOperatorType::possess:{
                this->printStStack.push(this->printStStack.pop()+comp1->getID()+this->printStStack.pop());
                break;
            }
            case BanSOperatorType::conveyed:{
                this->printStStack.push(this->printStStack.pop()+comp1->getID()+this->printStStack.pop());
                break;
            }
            case BanSOperatorType::hasJurisdiction:{
                this->printStStack.push(this->printStStack.pop()+comp1->getID()+this->printStStack.pop());
                break;
            }
            }
            break;
        }
        case BanSComponentType::bStatement:
        {
            BanStatementList *d1=dynamic_cast<BanStatementList *>(ptr);
            foreach(QString val, d1->getPrintStStack())
            {
                this->printStStack.push(val);
            }
            break;
        }
        }
    }
}
void BANLogic::BanStatementList::printRPN()
{
    QTextStream(stdout)<<"{";
    foreach(BanSComponent *ptr,this->stList)
    {
        switch(ptr->getStype())
        {
        case BanSComponentType::bData:
        {
            BanDataList *d1=dynamic_cast<BanDataList *>(ptr);
            d1->printRPN();
            break;
        }
        case BanSComponentType::bSOperator:
        {
            BanSOperator *d1=dynamic_cast<BanSOperator *>(ptr);
            d1->printRPN();
            break;
        }
        case BanSComponentType::bStatement:
        {
            BanStatementList *d1=dynamic_cast<BanStatementList *>(ptr);
            if(d1->getInstantiate()==false)
                QTextStream(stdout)<<"{"<<d1->getID()<<"}"<<endl;
            else
                d1->printRPN();
            break;
        }
        }
        //QTextStream(stdout)<<ptr->getID();
    }
    QTextStream(stdout)<<"}";
    cout<<endl;
}

bool BANLogic::BanStatementList::match(BanSComponent &Scomp)
{
    switch(Scomp.getStype())
    {
    case BanSComponentType::bStatement:
    {
        BanStatementList data=dynamic_cast<BanStatementList&>(Scomp);
        QList<BanSComponent*> mylist1=this->getStList();
        QList<BanSComponent*> mylist2=data.getStList();
        QList<BanDComponent*> temp1;
        QList<BanSComponent*> temp;
        int mycount=0, juno=0,dataTypeIndex=0;
        if(mylist1.size()==mylist2.size())
        {
            for(int i=0; i<mylist1.size(); i++)
            {
                switch(mylist1.value(i)->getStype())
                {
                case BanSComponentType::bData:
                {
                    BanDataList *d1=dynamic_cast<BanDataList*>(mylist1.value(i));
                    BanDataList *d2=dynamic_cast<BanDataList*>(mylist2.value(i));
                    if(d1->match(*d2))
                    {
                        ifMatches=true;
                        mycount++;
                    }
                    else
                    {
                        ifMatches=false;
                        break;
                    }
                    break;
                }
                case BanSComponentType::bSOperator:
                {
                    BanSOperator *op1=dynamic_cast<BanSOperator*>(mylist1.value(i));
                    BanSOperator *op2=dynamic_cast<BanSOperator*>(mylist2.value(i));
                    if(op1->match(*op2))
                    {
                        ifMatches=true;
                        mycount++;
                    }
                    else
                    {
                        ifMatches=false;
                        break;
                    }
                    break;
                }
                case BanSComponentType::bStatement:
                {
                    BanStatementList *s1=dynamic_cast<BanStatementList*>(mylist1.value(i));
                    BanStatementList *s2=dynamic_cast<BanStatementList*>(mylist2.value(i));
                    if(s1->match(*s2))
                    {
                        ifMatches=true;
                        mycount++;
                    }
                    else
                    {
                        ifMatches=false;
                        break;
                    }
                    break;
                }
                }
            }
            if(mycount==mylist2.size())
            {
                ifMatches=true;
            }
            else ifMatches=false;
        }
        else if(mylist1.size()!=mylist2.size())
        {
            int i;
            for(i=0; i<mylist1.size(); i++)
            {
                if(mylist2.value(juno)->getStype()==mylist1.value(i)->getStype())
                {
                    switch(mylist1.value(i)->getStype())
                    {
                    case BanSComponentType::bData:
                    {
                        BanDataList *d1=dynamic_cast<BanDataList*>(mylist1.value(i));
                        BanDataList *d2=dynamic_cast<BanDataList*>(mylist2.value(i));
                        if(d1->match(*d2))
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
                        break;
                    }
                    case BanSComponentType::bSOperator:
                    {
                        BanSOperator *op1=dynamic_cast<BanSOperator*>(mylist1.value(i));
                        BanSOperator *op2=dynamic_cast<BanSOperator*>(mylist2.value(juno));
                        if(op1->match(*op2))
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
                        break;
                    }
                    case BanSComponentType::bStatement:
                    {
                        BanStatementList *s1=dynamic_cast<BanStatementList*>(mylist1.value(i));
                        BanStatementList *s2=dynamic_cast<BanStatementList*>(mylist2.value(i));
                        if(s1->match(*s2))
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
                        break;
                    }
                    }
                }
                else if(mylist2.value(juno)->getStype()!=mylist1.value(i)->getStype())
                {

                    if(mylist1.value(i)->getStype()==BanSComponentType::bStatement)
                    {
                        //saving the index of anyStatement
                        if(mylist1.at(i)->getStype()==BanSComponentType::bStatement)
                        {dataTypeIndex=i;}

                        for(int j=i; j<=mylist2.size()-(mylist1.size()-i);j++)
                        {
                            juno=j;

                            switch(mylist2.value(j)->getStype())
                            {
                            case BanSComponentType::bData:
                            {
                                if(mylist2.value(j)->getInstantiate()==true)
                                {
                                    temp.append(mylist2.value(j));
                                    BanDataList *dlist1=dynamic_cast<BanDataList*>(mylist2.value(j));
                                    foreach(BanDComponent *ptr ,dlist1->getDataList())
                                    {
                                        temp1.append(ptr);
                                    }
                                    mycount++;
                                    ifMatches=false;
                                }
                                break;
                            }
                            case BanSComponentType::bSOperator:
                            {
                                BanSOperator *sop=dynamic_cast<BanSOperator*>(mylist2.value(j));
                                temp.append(mylist2.value(j));
                                mycount++;
                                ifMatches=false;
                                break;
                            }

                            }
                        }
                        juno++;
                    }
                }
            }
            if(mycount==mylist2.size())
            {
                cout<<endl<<endl;

                QTextStream(stdout)<<this->getStList().at(dataTypeIndex)->getID();
                BanStatementList *tp=new BanStatementList({temp});
                cout<<" = "; tp->print();
                cout<<endl;
                ifMatches=true;
            }
            else ifMatches=false;
        }

        break;
    }
    case BanSComponentType::bData:{
        ifMatches=false;
        break;
    }
    case BanSComponentType::bSOperator:{
        ifMatches=false;
        break;
    }

    }

    return ifMatches;
}

bool BANLogic::BanStatementList::unify(BanSComponent &Scomp)
{
    if(this->match(Scomp))
    {
        switch(Scomp.getStype())
        {
        case BanSComponentType::bStatement:
        {
            BanStatementList data=dynamic_cast<BanStatementList&>(Scomp);
            QList<BanSComponent*> mylist1=this->getStList();
            QList<BanSComponent*> mylist2=data.getStList();
            QList<BanDComponent*> temp1;
            QList<BanSComponent*> temp;
            int mycount=0, juno=0,dataTypeIndex=0;
            if(mylist1.size()==mylist2.size())
            {
                for(int i=0; i<mylist1.size(); i++)
                {
                    switch(mylist1.value(i)->getStype())
                    {
                    case BanSComponentType::bData:
                    {
                        BanDataList *d1=dynamic_cast<BanDataList*>(this->stList.value(i));
                        BanDataList *d2=dynamic_cast<BanDataList*>(mylist2.value(i));

                        if(d1->unify(*d2))
                        {
                            QTextStream(stdout) <<d1->getID()+ " " <<flush;

                            unifies=true;
                            mycount++;
                        }
                        else
                        {
                            unifies=false;
                            break;
                        }
                        break;
                    }
                    case BanSComponentType::bSOperator:
                    {
                        BanSOperator *op1=dynamic_cast<BanSOperator*>(mylist1.value(i));
                        BanSOperator *op2=dynamic_cast<BanSOperator*>(mylist2.value(i));
                        if(op1->unify(*op2))
                        {
                            unifies=true;
                            mycount++;
                        }
                        else
                        {
                            unifies=false;
                            break;
                        }
                        break;
                    }
                    case BanSComponentType::bStatement:
                    {
                        BanStatementList *s1=dynamic_cast<BanStatementList*>(mylist1.value(i));
                        BanStatementList *s2=dynamic_cast<BanStatementList*>(mylist2.value(i));
                        if(s1->unify(*s2))
                        {
                            unifies=true;
                            mycount++;
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
                if(unifies)
                {
                    this->stList=data.stList;
                    this->printStStack=data.printStStack;
                }
                else unifies=false;
            }
            else if(mylist1.size()!=mylist2.size())
            {
                int i;
                for(i=0; i<mylist1.size(); i++)
                {
                    if(mylist2.value(juno)->getStype()==mylist1.value(i)->getStype())
                    {
                        switch(mylist1.value(i)->getStype())
                        {
                        case BanSComponentType::bData:
                        {
                            BanDataList *d1=dynamic_cast<BanDataList*>(mylist1.value(i));
                            BanDataList *d2=dynamic_cast<BanDataList*>(mylist2.value(juno));
                            if(d1->unify(*d2))
                            {

                                //this->getStList().value(i)->setId(mylist2.value(juno)->getID());

                                unifies=true;
                                mycount++;
                                juno++;
                            }
                            else
                            {
                                unifies=false;
                                break;
                            }
                            break;
                        }
                        case BanSComponentType::bSOperator:
                        {
                            BanSOperator *op1=dynamic_cast<BanSOperator*>(mylist1.value(i));
                            BanSOperator *op2=dynamic_cast<BanSOperator*>(mylist2.value(juno));
                            if(op1->unify(*op2))
                            {
                                unifies=true;
                                mycount++;
                                juno++;
                            }
                            else
                            {
                                unifies=false;
                                break;
                            }
                            break;
                        }
                        case BanSComponentType::bStatement:
                        {
                            BanStatementList *s1=dynamic_cast<BanStatementList*>(mylist1.value(i));
                            BanStatementList *s2=dynamic_cast<BanStatementList*>(mylist2.value(i));
                            if(s1->unify(*s2))
                            {
                                unifies=true;
                                mycount++;
                                juno++;
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
                    else if(mylist2.value(juno)->getStype()!=mylist1.value(i)->getStype())
                    {

                        if(mylist1.value(i)->getStype()==BanSComponentType::bStatement)
                        {
                            //saving the index of anyStatement
                            if(mylist1.at(i)->getStype()==BanSComponentType::bStatement)
                            {dataTypeIndex=i;}

                            for(int j=i; j<=mylist2.size()-(mylist1.size()-i);j++)
                            {
                                juno=j;

                                switch(mylist2.value(j)->getStype())
                                {
                                case BanSComponentType::bData:
                                {
                                    if(mylist2.value(j)->getInstantiate()==true)
                                    {
                                        temp.append(mylist2.value(j));
                                        BanDataList *dlist1=dynamic_cast<BanDataList*>(mylist2.value(j));
                                        foreach(BanDComponent *ptr ,dlist1->getDataList())
                                        {
                                            temp1.append(ptr);
                                        }
                                        mycount++;
                                        unifies=false;
                                    }
                                    break;
                                }
                                case BanSComponentType::bSOperator:
                                {
                                    BanSOperator *sop=dynamic_cast<BanSOperator*>(mylist2.value(j));
                                    temp.append(mylist2.value(j));
                                    mycount++;
                                    unifies=false;
                                    break;
                                }

                                }
                            }
                            juno++;
                        }
                    }
                }
                if(unifies)
                {
                    BanStatementList *tp=new BanStatementList(temp);
                    this->stList.replace(dataTypeIndex,tp);
                    this->printStStack=tp->printStStack;

                }
            }

            break;
        }
        case BanSComponentType::bData:{
            ifMatches=false;
            break;
        }
        case BanSComponentType::bSOperator:{
            ifMatches=false;
            break;
        }

        }


    }
    return unifies;
}

bool BanStatementList::getInstantiate() const
{
    return instantiate;
}

bool BanStatementList::getIfMatches() const
{
    return ifMatches;
}
void BANLogic::BanStatementList::print()
{
    BanStatementList *tp=new BanStatementList(this->stList);

    foreach(QString ptr, tp->printStStack)
    {
        QTextStream(stdout)<<ptr;
    }
}


void BANLogic::BanStatementList::setId(const QString &value)
{
    stid=value;
}
