#include "banstatementlist.h"
using namespace BANLogic;
namespace BANLogic {

int BANLogic::BanStatementList::statementCount=1;

QList<BanSComponent *> BanStatementList::getStList() const
{
    return stList;
}

QStack<QString> BanStatementList::getPrintStStack() const
{
    return printStStack;
}

BanStatementList::BanStatementList(BanStatementList &orig):BanSComponent(BanSComponentType::bStatement)
{
    BanSOperator *oper;
    BanDataList *datas;
    switch(orig.stype)
    {
    case BanSComponentType::bStatement:
    {
        stid=orig.stid;
        stype= orig.stype;
        stList=orig.stList;
        statementCount=orig.statementCount;
        instantiate=orig.instantiate;
        printStStack=orig.printStStack;
        break;
    }
    case BanSComponentType::bSOperator:
    {
        BanSOperator *o=dynamic_cast<BanSOperator*>(&orig);
        oper=o;
        break;
    }
    case BanSComponentType::bData:
    {
        BanDataList *dList=dynamic_cast<BanDataList*>(&orig);
        datas=dList;
        break;
    }
    }
}



BANLogic::BanStatementList::BanStatementList():BanSComponent(BanSComponentType::bStatement)
{
    if(this->stid=="" || this->stid.isEmpty() || this->stid.isNull())
    {
        this->stid=QString("_St_%1").arg(statementCount++);
        this->instantiate=false;
        //stList.append(this);
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
            case BanSOperatorType::said:{
                this->printStStack.push(this->printStStack.pop()+comp1->getID()+this->printStStack.pop());
                break;
            }
            case BanSOperatorType::believes:{
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
            case BanSOperatorType::controls:{
                this->printStStack.push(this->printStStack.pop()+comp1->getID()+this->printStStack.pop());
                break;
            }
            case BanSOperatorType::sees:{
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
    ifMatches=false;
    BanStatementList data=dynamic_cast<BanStatementList&>(Scomp);
    QList<BanSComponent*> mylist1=this->stList;
    QList<BanSComponent*> mylist2=data.stList;
    int mycount=0;
    switch(Scomp.getStype())
    {
    case BanSComponentType::bStatement:
    {
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
                    else if(s1->getInstantiate()==false && s2->getInstantiate()==true)
                    {
                        ifMatches=true;
                        mycount++;
                    }
                    break;
                }

                }
                if(mycount==mylist2.size())
                {
                    ifMatches=true;
                }
                else ifMatches=false;
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

    return ifMatches;
}

bool BANLogic::BanStatementList::unify(BanSComponent &Scomp)
{
    unifies=false;

    BanStatementList data=dynamic_cast<BanStatementList&>(Scomp);
    QList<BanSComponent*> mylist1=this->stList;
    QList<BanSComponent*> mylist2=data.stList;
    QList<BanDComponent*> temp1;
    QList<BanSComponent*> temp;
    int mycount=0;

    if(this->match(Scomp))
    {

        switch(Scomp.getStype())
        {
        case BanSComponentType::bStatement:
        {
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

                        if(d1->unify(*d2))
                        {
                            QTextStream(stdout) <<d1->getID()+ " " <<flush;
                            unifies=true;
                            //this->stList.replace(i,d2);
                            //this->stList.value(i)->setId(d2->getID());
                            temp.append(d2);

                            mycount++;
                        }
                        else
                        {
                            unifies=false;
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
                        }
                        break;
                    }
                    case BanSComponentType::bStatement:
                    {
                        BanStatementList *s1=dynamic_cast<BanStatementList*>(mylist1.value(i));
                        BanStatementList *s2=dynamic_cast<BanStatementList*>(mylist2.value(i));
                        {
                            if(s1->stList.isEmpty())
                            {
                                s1->stList.append(mylist2.value(i));
                                s1->printStStack=s2->printStStack;
                            }
                            else
                            {
                                if(s1->unify(*s2))
                                {
                                    unifies=true;
                                    mycount++;
                                }
                            }
                            unifies=true;
                            mycount++;
                        }

                        break;
                    }
                    }
                }
            }
            break;
        }
        case BanSComponentType::bData:{
            unifies=false;
            break;
        }
        case BanSComponentType::bSOperator:{
            unifies=false;
            break;
        }
        }
    }
    if(unifies==true)
    {
        BanStatementList *final=new BanStatementList(this->stList);
        this->printStStack=final->printStStack;
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
    //BanStatementList *tp=new BanStatementList(this->stList);
    foreach(QString ptr, this->printStStack)
    {
        QTextStream(stdout)<<ptr;
    }
}

void BANLogic::BanStatementList::setId(const QString &value)
{
    stid=value;
}

bool BANLogic::BanStatementList::operator ==(const LPT::Statement &stmt)
{
    bool equals = true;
    const BanStatementList *s = dynamic_cast<const BanStatementList *>(&stmt);
    if (s == nullptr) {
        // if passed stmt is not ban statement, comparison fails
        equals = false;
    } else {
        equals = *this == dynamic_cast<const BanSComponent &>(stmt);
    }
    return equals;
}

QString BANLogic::BanStatementList::getString()
{
    QString s="";
    BanStatementList *d=new BanStatementList(this->getStList());
    if(d->printStStack.size()==1)
    {
        s.append(d->printStStack.top());
    }
    else
    {
        throw new BanException("Incorrect number of arguments passed, please check the parameters properly: :BanDataList::getString()");
    }
    return s;
}


bool BANLogic::BanStatementList::operator ==(const BanSComponent &Scomp)
{
    bool equals=false;
    const BanStatementList &data=dynamic_cast<const BanStatementList&>(Scomp);
    QList<BanSComponent*> mylist1=this->stList;
    QList<BanSComponent*> mylist2=data.stList;
    int mycount=0;
    switch(Scomp.getStype())
    {
    case BanSComponentType::bStatement:
    {
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
                    if(d1->operator==(*d2))
                    {
                        equals=true;
                        mycount++;
                    }
                    else
                    {
                        equals=false;
                    }
                    break;
                }
                case BanSComponentType::bSOperator:
                {
                    BanSOperator *op1=dynamic_cast<BanSOperator*>(mylist1.value(i));
                    BanSOperator *op2=dynamic_cast<BanSOperator*>(mylist2.value(i));
                    if(op1->operator==(*op2))
                    {
                        equals=true;
                        mycount++;
                    }
                    else
                    {
                        equals=false;
                    }
                    break;
                }
                case BanSComponentType::bStatement:
                {
                    BanSComponent *s1=dynamic_cast<BanStatementList*>(mylist1.value(i));
                    BanSComponent *s2=dynamic_cast<BanStatementList*>(mylist2.value(i));
                    if(s1->operator ==(*s2))
                    {
                        equals=true;
                        mycount++;
                    }
                    else if(s1->getInstantiate()==false && s2->getInstantiate()==true)
                    {
                        equals=true;
                        mycount++;
                    }
                    break;
                }

                }
                if(mycount==mylist2.size())
                {
                    equals=true;
                }
                else equals=false;
            }
        }
        break;
    }
    case BanSComponentType::bData:{
        equals=false;
        break;
    }
    case BanSComponentType::bSOperator:{
        equals=false;
        break;
    }
    }
    return equals;
}
}
