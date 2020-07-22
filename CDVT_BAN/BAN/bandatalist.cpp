#include "bandatac.h"
#include "bandatalist.h"
using namespace BANLogic;
namespace BANLogic {

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
    BanDataList *d=new BanDataList(this->getDataList());
    if(d->getPrintStack().size()==1)
    {
        //        foreach(QString val,this->getPrintStack())
        //        {
        QTextStream(stdout)<<d->getPrintStack().top();
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
                this->printStack.push(ptr->getID()+"("+this->printStack.pop()+")");
                //this->dataList.append(myOperator);
                break;
            }
            case BanDOperatorType::ShareKey:{
                this-> printStack.push(this->printStack.pop()+","+this->printStack.pop()+ptr->getID()+this->printStack.pop());
                break;
            }
            case BanDOperatorType::ShareSecret:{
                this-> printStack.push(this->printStack.pop()+","+this->printStack.pop()+ptr->getID()+this->printStack.pop());
                break;
            }
            case BanDOperatorType::HasKey:{
                this-> printStack.push(this->printStack.pop()+ptr->getID()+this->printStack.pop());
                break;
            }
            case BanDOperatorType::SecretPassword:{
                this-> printStack.push("<"+this->printStack.pop()+">"+this->printStack.pop());
                break;
            }
            default:
            {
                throw new BanException("please check the parameters properly:BanDataList bOperator");
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
            break;
        }
        default:
        {
            throw new BanException("please check the parameters properly:BanDataList");
        }
        }

    }
}
BanDataList *BanDataList::getCopy(LPT::LPTPtrList<BanDComponent> &components)
{
    BanDAtom *Prin,*a;
    BanDataList *data2;
    BanDataList *copy =new BanDataList();
    copy->stype=stype;
    BanDComponent *c;
    BanDataC *Dc;

    BanDOperator *op2;
    switch(stype)
    {
    case BanSComponentType::bData:
    {
        if(!dataList.isEmpty())
        {
            for(int i=0; i<dataList.count(); ++i)
            {
                switch(dataList.at(i)->getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    if(i==0)
                    {
                        Prin= dynamic_cast<BanDAtom *>(this->getDataList().value(0));
                        for (int j=0; j<components.count(); ++j) {
                            c = components.at(j);
                            if (c->getDtype() == BanDComponentType::bAtom) {
                                a = dynamic_cast<BanDAtom *>(c);
                                if (*a == *Prin) {
                                    // found component, set it in copy
                                    copy->dataList.append(a);
                                    break;
                                }
                            }
                        }
                    }else{
                        for (int k=0; k<components.count(); ++k) {
                            c = components.at(k);
                            if ( (c->getDtype() == BanDComponentType::bAtom) && (*c == *dataList.at(i))) {
                                // found component, set it in copy
                                copy->dataList.append(c);
                                break;
                            }
                        }
                    }
                    break;
                }
                case BanDComponentType::bOperator:
                {
                    op2=dynamic_cast<BanDOperator*>(dataList.at(i));
                    copy->dataList.append(op2);
                    break;
                }
                case BanDComponentType::bAnyData:
                {
                    Dc=  dynamic_cast<BanDataC *>(dataList.at(i));//->getCopy(components);
                    copy->dataList.append(Dc);
                    break;
                }

                }
            }
        }
        break;
    }
    case BanSComponentType::bStatement:
    case BanSComponentType::bSOperator:
        break;
    }
    copy->dataID=dataID;
    copy->unifies=unifies;
    copy->printStack=printStack;
    return copy;
}

BanDataList::BanDataList(BanDataList &orig):BanSComponent(BanSComponentType::bData)
{
    BanDAtom *a1;
    BanDOperator *op1;
    BanDataC *dc1;
    switch(orig.stype)
    {
    case BanSComponentType::bData:
    {
        dataList=orig.dataList;
        setId(orig.getID());
        printStack=orig.printStack;
        instantiate= orig.getInstantiate();
        stype=orig.stype;
        foreach(BanDComponent *ptr,dataList)
        {
            switch(ptr->getDtype())
            {
            case BanDComponentType::bAtom:
            {
                BanDAtom *d1=dynamic_cast<BanDAtom *>(ptr);
                a1=d1;
                break;
            }
            case BanDComponentType::bOperator:
            {
                BanDOperator *d1=dynamic_cast<BanDOperator *>(ptr);
                op1=d1;
                break;
            }
            case BanDComponentType::bAnyData:
            {
                BanDataC *d1=dynamic_cast<BanDataC *>(ptr);
                dc1=d1;
                break;
            }
            }

        }
        break;
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
        if(this->dataList.size()==data.dataList.size())
        {
            int length=this->dataList.size();
            for(int i=0; i<length; i++)
            {
                if(this->dataList.at(i)->getDtype()==data.dataList.at(i)->getDtype())
                {
                    switch(this->dataList.at(i)->getDtype())
                    {
                    case BanDComponentType::bAtom:
                    {
                        if(this->dataList.value(i)->match(*data.dataList.value(i)))
                        {
                            ifMatches=true;
                        }
                        else ifMatches=false;
                        break;
                    }
                    case BanDComponentType::bOperator:
                    {

                        if(this->dataList.value(i)->match(*data.dataList.value(i)))
                            ifMatches=true;
                        else ifMatches=false;

                        break;
                    }
                    case BanDComponentType::bAnyData:
                    {
                        if(this->dataList.value(i)->getDtype()==data.dataList.value(i)->getDtype())
                        {
                            if(this->dataList.value(i)->match(*data.dataList.value(i)))
                                ifMatches=true;
                            else ifMatches=false;
                        }
                        else ifMatches=false;

                        break;
                    }
                    }
                }
            }
        }
        else if(this->dataList.size()!=data.dataList.size())
        {
            QList<BanDComponent*> temp;

            int i, mycount=0, juno=0,dataTypeIndex=0;
            for(i=0;i<this->dataList.size();i++)
            {
                if(data.dataList.value(juno)->getDtype()==this->dataList.value(i)->getDtype())
                {
                    if(data.dataList.value(juno)->getInstantiate()==true && this->dataList.value(i)->getInstantiate()==false )
                    {
                        if(this->dataList.value(i)->match(*(data.dataList.value(juno))))
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
                    else if(data.dataList.value(juno)->getDtype()==BanDComponentType::bOperator && this->dataList.value(i)->getDtype()==BanDComponentType::bOperator )
                    {
                        if(this->dataList.value(i)->match(*(data.dataList.value(juno))))
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
                else if(data.dataList.value(juno)->getDtype()!=this->dataList.value(i)->getDtype())
                {
                    if(this->dataList.value(i)->getDtype()==BanDComponentType::bAnyData)
                    {
                        for(int j=i; j<=data.dataList.size()-(this->dataList.size()-i);j++)
                        {
                            juno=j;

                            if(this->dataList.value(i)->match(*data.dataList.value(j)))
                            {
                                ifMatches=true;
                                temp.append(data.dataList.value(j));
                                mycount++;
                            }
                            else
                            {
                                ifMatches=false;
                                mycount++;
                                // break;
                            }

                            juno++;
                        }
                    }
                }
            }
            if(mycount==data.dataList.size())
            {
                //QTextStream(stdout)<<this->getDataList().at(dataTypeIndex)->getID()<< " =  ";
                //                foreach(BanDComponent *lo, temp)
                //                {
                //                    //QTextStream(stdout) <<lo->getID()+"  "<<flush;
                //                }
                ifMatches=true;
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
        int dataTypeIndex=0;
        QList<BanDComponent*> temp;
        int l=this->dataList.length();
        switch(Scomp.getStype())
        {
        case BanSComponentType::bData:
        {
            BanDataList &data=dynamic_cast<BanDataList&>(Scomp);
            if(this->dataList.size()==data.dataList.size()){
                for(int i=0; i<l; i++)
                {
                    switch(this->dataList.at(i)->getDtype())
                    {
                    case BanDComponentType::bAtom:
                    {
                        BanDAtom *atom1=dynamic_cast<BanDAtom*>(this->dataList.value(i));
                        BanDAtom *atom2=dynamic_cast<BanDAtom*>(data.dataList.value(i));
                        if(atom1->unify(*atom2))
                        {
                            temp.append(data.dataList.value(i));
                            //this->dataList.value(i)->setId(atom2->getID());
                            this->dataList.replace(i,data.dataList.value(i));
                            this->printStack.push(data.dataList.value(i)->getID());

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
                        if(this->dataList.value(i)->unify(*data.dataList.value(i)))
                        {
                            temp.append(data.dataList.value(i));
                            this->dataList.replace(i,data.dataList.value(i));
                            this->printStack.push(data.dataList.value(i)->getID());

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
                        BanDataC *dataany1=dynamic_cast<BanDataC *>(this->dataList.value(i));
                        QTextStream(stdout)<<dataany1->getID()+" = "<<flush;
                        if(dataany1->unify(*data.dataList.value(i)))
                        {
                            temp.append(data.dataList.value(i));
                            this->dataList.replace(i,data.dataList.value(i));
                            this->printStack.push(data.dataList.value(i)->getID());

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
            else if(this->dataList.size()!=data.dataList.size())
            {

                int i, mycount=0, juno=0;
                for(i=0;i<this->dataList.size();i++)
                {
                    if(data.dataList.value(juno)->getDtype()==this->dataList.value(i)->getDtype())
                    {

                        if(this->dataList.value(i)->unify(*data.dataList.value(juno)))
                        {
                            unifies=true;
                            //this->dataList.value(i)->setId(mylist2.value(juno)->getID());
                            temp.append(data.dataList.value(juno));
                            this->dataList.replace(i,data.dataList.value(juno));
                            this->printStack.push(data.dataList.value(juno)->getID());

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
                    else if(data.dataList.value(i)->getDtype()!=this->dataList.value(i)->getDtype())
                    {
                        QTextStream(stdout)<<this->dataList.value(i)->getID()+ " = " <<flush;

                        if(this->dataList.value(i)->getDtype()==BanDComponentType::bAnyData)
                        {
                            dataTypeIndex=i;

                            for(int j=i; j<=data.dataList.size()-(this->dataList.size()-i);j++)
                            {
                                juno=j;
                                if(this->dataList.value(i)->unify(*data.dataList.value(j)))
                                {
//                                    if(dataList.at(i)->getDtype()==BanDComponentType::bAnyData)
//                                    {
//                                        this->dataList.pop_back();
//                                         this->printStack.pop();
//                                    }
                                    //temp.append(data.dataList.value(j));
                                    //this->dataList.append(data.dataList.value(j));
                                    this->printStack.push(data.dataList.value(j)->getID());

                                    unifies=true;
                                    mycount++;
                                }

                                juno++;
                            }
                        }
                    }
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
        if(unifies)
        {
            //this->dataList=temp;
           // BanDataList *d=new BanDataList(this->dataList);
//            this->printStack=d->printStack;
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

void BanDataList::setId(const QString &value)
{
    dataID=value;
}

QString BANLogic::BanDataList::getString()
{
    QString s="";
//    BanDataList *d=new BanDataList(this->getDataList());

//        s.append(d->getPrintStack().top());

        foreach(BanDComponent *ptr,this->getDataList())
        {
            switch(ptr->getDtype())
            {
            case BanDComponentType::bAtom:
            {
                BanDAtom *d1=dynamic_cast<BanDAtom *>(ptr);
                s.append(d1->getString());
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
                BanDataC *d1=dynamic_cast<BanDataC *>(ptr);
                s.append(d1->getString());
                break;
            }
            }
}
            //QTextStream(stdout)<<ptr->getID();

    return s;
}

bool BANLogic::BanDataList::operator ==(const BanSComponent &other)
{
    int count=0;
    bool equals=true;
    switch(other.getStype())
    {
    case BanSComponentType::bData:
    {
        const BanDataList &data=dynamic_cast<const BanDataList&>(other);
        if(this->getDataList().size()==data.dataList.size())
        {
            int length=this->getDataList().size();
            for(int i=0; i<length; i++)
            {
                if(this->getDataList().at(i)->getDtype()==data.dataList.at(i)->getDtype())
                {
                    switch(this->getDataList().at(i)->getDtype())
                    {
                    case BanDComponentType::bAtom:
                    {
                        BanDAtom *atom1=dynamic_cast<BanDAtom *>(this->getDataList().value(i));
                        BanDAtom *atom2=dynamic_cast<BanDAtom *>(data.dataList.value(i));
                        if(atom1->operator==(*atom2))
                        {
                            equals=true;
                            count++;
                        }
                        else
                        {
                            equals=false;
                            break;
                        }
                        break;
                    }
                    case BanDComponentType::bOperator:
                    {
                        BanDOperator *operator1=dynamic_cast<BanDOperator *>(this->getDataList().value(i));
                        BanDOperator *operator2=dynamic_cast<BanDOperator *>(data.dataList.value(i));
                        if(operator1->getDOtype()==operator2->getDOtype())
                        {
                            if(operator1->operator==(*operator2))
                            {
                                equals=true;
                                count++;
                            }
                            else
                            {
                                equals=false;
                                break;
                            }
                        }
                        else equals=false;

                        break;
                    }
                    case BanDComponentType::bAnyData:
                    {
                        BanDataC *d1=dynamic_cast<BanDataC *>(this->getDataList().value(i));
                        BanDataC *d2=dynamic_cast<BanDataC *>(data.dataList.value(i));
                        if(d1->getDtype()==d2->getDtype())
                        {
                            if(d1->operator==(*d2))
                            {equals=true;
                                count++;
                            }
                            else
                            {
                                equals=false;
                                break;
                            }
                        }
                        break;
                    }
                    }
                }
            }
            if(count==length)
            {
                equals=true;
            }
            else
            {
                equals=false;
                break;
            }
        }
        else if(this->getDataList().size()!=data.dataList.size())
        {
            equals=false;
            //            QList<BanDComponent*> mylist1=this->getDataList();
            //            QList<BanDComponent*> mylist2=data.dataList;
            //            QList<BanDComponent*> temp;

            //            int i, mycount=0, juno=0,dataTypeIndex=0;
            //            for(i=0;i<mylist1.size();i++)
            //            {
            //                if(mylist2.value(juno)->getDtype()==mylist1.value(i)->getDtype())
            //                {
            //                    if(mylist2.value(juno)->getInstantiate()==true && mylist1.value(i)->getInstantiate()==false )
            //                    {
            //                        if(mylist1.value(i)->operator==(*mylist2.value(juno)))
            //                        {
            //                            equals=true;
            //                            mycount++;
            //                            juno++;
            //                        }
            //                        else
            //                        {
            //                            equals=false;
            //                            break;
            //                        }
            //                    }
            //                    else if(mylist2.value(juno)->getDtype()==BanDComponentType::bOperator && mylist1.value(i)->getDtype()==BanDComponentType::bOperator )
            //                    {
            //                        if(mylist1.value(i)->operator==(*mylist2.value(juno)))
            //                        {
            //                            equals=true;
            //                            mycount++;
            //                            juno++;
            //                        }
            //                        else
            //                        {
            //                            equals=false;
            //                            break;
            //                        }
            //                    }
            //                }
            //                else if(mylist2.value(i)->getDtype()!=mylist1.value(i)->getDtype())
            //                {
            //                    if(mylist1.value(i)->getDtype()==BanDComponentType::bAnyData)
            //                    {
            //                        for(int j=i; j<=mylist2.size()-(mylist1.size()-i);j++)
            //                        {
            //                            juno=j;

            //                            if(mylist1.value(i)->operator==(*mylist2.value(j)))
            //                            {
            //                                equals=true;
            //                                temp.append(mylist2.value(j));
            //                                mycount++;
            //                            }
            //                            else
            //                            {
            //                                equals=false;
            //                                mycount++;
            //                                // break;
            //                            }

            //                            juno++;
            //                        }
            //                    }
            //                }
            //            }
            //            if(mycount==mylist2.size())
            //            {
            //                equals=true;
            //            }
            //            else
            //            {
            //                equals=false;
            //            }
        }
        break;
    }
    case BanSComponentType::bSOperator:
    {
        equals=false;
        break;
    }
    case BanSComponentType::bStatement:
    {
        equals=false;
        break;
    }
    }
    return equals;
}
}
