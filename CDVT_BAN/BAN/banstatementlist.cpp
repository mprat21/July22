#include "bandatac.h"
#include "banstatementlist.h"
using namespace BANLogic;
namespace BANLogic {

int BANLogic::BanStatementList::statementCount=1;

BanStatementList *BanStatementList::getCopy(LPT::LPTPtrList<BanDComponent> &components)
{
    BanStatementList *copy = new BanStatementList();
    copy->printStStack.clear();
    BanDAtom *a, *Prin;
    BanDataList *data1, *data2;
    BanDataC *dc1, *dc2;
    BanDComponent *c;
    BanStatementList *Sc;
    BanSOperator *op1,*op2;
    QString s="";

    copy->stype = stype;
    if(!this->stList.isEmpty() )
    {
        for(int i=0; i<stList.count(); i++)
        {
            switch(stList.value(i)->getStype())
            {
            case BanSComponentType::bData:
            {
                data1=dynamic_cast<BanDataList *>(stList.value(i));
                copy->stList.append(data1->getCopy(components));
                //data2=new BanDataList();

                //                if(i==0)
                //                {
                //                    Prin= dynamic_cast<BanDAtom *>(dynamic_cast<BanDataList *>(this->stList.value(0))->getDataList().value(0));
                //                    for (int j=0; j<components.count(); ++j) {
                //                        c = components.at(j);
                //                        if (c->getDtype() == BanDComponentType::bAtom) {
                //                            a = dynamic_cast<BanDAtom *>(c);
                //                            if (*a == *Prin) {
                //                                // found component, set it in copy
                //                                data2=new BanDataList({a});
                //                                copy->stList.append(data2);
                //                               // copy->printStStack.push(data2->getString());
                //                                break;
                //                            }
                //                        }
                //                    }
                //                }
                //                else
                //                {
                //                    for(int k=0; k<data1->getDataList().size(); k++)
                //                    {
                //                        for (int j=0; j<components.count(); ++j) {
                //                            c = components.at(j);
                //                            if ( (c->getDtype() == BanDComponentType::bAnyData || c->getDtype() == BanDComponentType::bAtom) && *c==*data1->getDataList().at(k) )
                //                            {
                //                                copy->stList.append(data1);
                //                                break;
                //                            }
                //                        }
                //                       // copy->printStStack.push(data1->getString());
                //                    }
                //                }
                break;
            }
            case BanSComponentType::bSOperator:
            {
                op2=dynamic_cast<BanSOperator*>(stList.value(i));
                copy->stList.append(op2);
                // copy->printStStack.push(op2->getString());
                break;
            }
            case BanSComponentType::bStatement:
            {
                Sc=  dynamic_cast<BanStatementList *>(stList.value(i));
                copy->stList.append(Sc->getCopy(components));
                // copy->printStStack.push(Sc->getString());
                break;
            }
            }
        }

    }
    copy->stid = stid;
    copy->printStStack=printStStack;
    return copy;
}




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
    //  stype=orig.stype;
    //stid=orig.stid;
    // stList=orig.stList;
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



void BanStatementList::setStList(const QList<BanSComponent *> &value)
{
    stList = value;
}

void BanStatementList::findData(QList<BanDComponent*> sList) const
{
    this->stList;
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

    int mycount=0;
    switch(Scomp.getStype())
    {
    case BanSComponentType::bStatement:
    {
        BanStatementList data=dynamic_cast<BanStatementList&>(Scomp);
        QList<BanSComponent*> mylist1=this->stList;
        QList<BanSComponent*> mylist2=data.stList;
        if(mylist1.size()==mylist2.size())
        {
            for(int i=0; i<mylist1.size(); i++)
            {
                switch(mylist1.value(i)->getStype())
                {
                case BanSComponentType::bData:
                {
                    if(mylist1.value(i)->match(*mylist2.value(i)))
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

                    if(mylist1.value(i)->match(*mylist2.value(i)))
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
                    if(mylist1.value(i)->match(*mylist2.value(i)))
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

    QList<BanDComponent*> temp1;
    QList<BanSComponent*> temp;
    int mycount=0;

    if(this->match(Scomp))
    {
        switch(Scomp.getStype())
        {
        case BanSComponentType::bStatement:
        {
            BanStatementList data=dynamic_cast<BanStatementList&>(Scomp);
            if(this->stList.size()==data.stList.size())
            {
                for(int i=0; i<this->stList.size(); i++)
                {
                    switch(this->stList.value(i)->getStype())
                    {
                    case BanSComponentType::bData:
                    {

                        if(this->stList.value(i)->unify(*data.stList.value(i)))
                        {
                            QTextStream(stdout) <<this->stList.value(i)->getID()+ " " <<flush;
                            unifies=true;
                            this->stList.replace(i,data.stList.value(i));
                            this->printStStack.push(data.stList.value(i)->getID());

                            //this->stList.value(i)->setId(d2->getID());
                            temp.append(data.stList.value(i));

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
                        if(this->stList.value(i)->unify(*data.stList.value(i)))
                        {
                            unifies=true;
                            this->stList.replace(i,data.stList.value(i));
                            this->printStStack.push(data.stList.value(i)->getID());
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
                        BanStatementList *s1=dynamic_cast<BanStatementList*>(this->stList.value(i));
                        BanStatementList *s2=dynamic_cast<BanStatementList*>(data.stList.value(i));
                        {
                            if(s1->stList.isEmpty())
                            {
                                s1->stList.append(data.stList.value(i));
                                s1->printStStack=s2->printStStack;
                                unifies=true;
                            }
                            else
                            {
                                if(s1->unify(*s2))
                                {
                                    s1->stList.append(data.stList.value(i));
                                    s1->printStStack.push(data.stList.value(i)->getID());
                                    unifies=true;
                                    mycount++;
                                }
                                else
                                {
                                    unifies=false;
                                    break;
                                }
                            }
                            //unifies=true;
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
        //BanStatementList *final=new BanStatementList(this->stList);
        //this->printStStack=final->printStStack;
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
    std::cout << getString().toStdString() << std::endl;

    //    BanStatementList *tp=new BanStatementList(this->stList);
    //    foreach(QString ptr, this->printStStack)
    //    {
    //        QTextStream(stdout)<<ptr;
    //    }
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
    BanStatementList *d=new BanStatementList(stList);
    s.append(d->printStStack.pop());


//    foreach(BanSComponent *ptr,this->stList)
//    {
//        switch(ptr->getStype())
//        {
//        case BanSComponentType::bData:
//        {
//            BanDataList *d1=dynamic_cast<BanDataList *>(ptr);
//            s.append(d1->getString());
//            break;
//        }
//        case BanSComponentType::bSOperator:
//        {
//            BanSOperator *d1=dynamic_cast<BanSOperator *>(ptr);
//            s.append(d1->getID());
//            break;
//        }
//        case BanSComponentType::bStatement:
//        {
//            BanStatementList *d1=dynamic_cast<BanStatementList *>(ptr);
//            s.append(d1->getString());
//            break;
//        }
//        }
//        //QTextStream(stdout)<<ptr->getID();
//    }

    return s;
}


bool BANLogic::BanStatementList::operator ==(const BanSComponent &Scomp)
{
    bool equals=false;

    int mycount=0;
    switch(Scomp.getStype())
    {
    case BanSComponentType::bStatement:
    {
        const BanStatementList &data=dynamic_cast<const BanStatementList&>(Scomp);
        if(this->stList.size()==data.stList.size())
        {
            for(int i=0; i<this->stList.size(); i++)
            {
                switch(this->stList.value(i)->getStype())
                {
                case BanSComponentType::bData:
                {
                    BanDataList *d1=dynamic_cast<BanDataList*>(this->stList.value(i));
                    if(d1->operator==(*data.stList.value(i)))
                    {
                        equals=true;
                        mycount++;
                    }
                    else
                    {
                        equals=false;
                        break;
                    }
                    break;
                }
                case BanSComponentType::bSOperator:
                {
                    BanSOperator *op1=dynamic_cast<BanSOperator*>(this->stList.value(i));
                    if(op1->operator==(*data.stList.value(i)))
                    {
                        equals=true;
                        mycount++;
                    }
                    else
                    {
                        equals=false;
                        break;

                    }
                    break;
                }
                case BanSComponentType::bStatement:
                {
                    BanSComponent *s1=dynamic_cast<BanStatementList*>(this->stList.value(i));

                    if(s1->operator ==(*data.stList.value(i)))
                    {
                        equals=true;
                        mycount++;
                    }
                    else
                    {
                        equals=false;
                        break;
                    }
                    break;
                }

                }
                if(mycount==data.stList.size())
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
