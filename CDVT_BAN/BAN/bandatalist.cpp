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


QStack<BanDComponent *> BanDataList::getOtherPart() const
{
    return otherPart;
}

void BanDataList::setOtherPart(const QStack<BanDComponent *> &value)
{
    otherPart = value;
}

BANLogic::BanDataList::BanDataList():BanSComponent(BanSComponentType::bData)
{
}

BANLogic::BanDataList::BanDataList(QList<BanDComponent *> dList):BanSComponent(BanSComponentType::bData)
{
    //this->instantiate=true;
    foreach(BanDComponent *ptr,dList)
    {
        switch(ptr->getDtype())
        {
        case BanDComponentType::bAtom:
        {
            BanDAtom * batom = dynamic_cast<BanDAtom *>(ptr);
            //BanDAtom *myatom= new BanDAtom(batom->getAtype(),batom->getID());
            instantiate=true;
           // this->setId(batom->getID());
            dataList.append(batom);
            this->printStack.push(batom->getID());
            break;
        }
        case BanDComponentType::bOperator:
        {
            instantiate=true;
            BanDOperator * myOperator = dynamic_cast<BanDOperator *>(ptr);
            dataList.append(myOperator);
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
            dataList.append(bdata);
            //this->setId(bdata->getID());
            this->printStack.append(bdata->getID());

            //            if(!bdata->getMyListdata().isEmpty())
            //            {
            //                this->printStack.push(bdata->getID());
            //                //this->instantiate=true;
            //                break;
            //            }
            //            else
            //            {
            //                BanDataList *mybn=new BanDataList(bdata->getMyListdata());
            //                foreach(QString val,mybn->getPrintStack())
            //                {
            //                    this-> printStack.push(val);
            //                    this->instantiate=true;
            //                }
            //            }
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
    BanDataList *copy =new BanDataList();
    copy->stype=stype;

    BanDComponent *c;
    BanDAtom *Prin,*a;

    BanDataC *Dc;

    BanDOperator *op2;
    switch(stype)
    {
    case BanSComponentType::bData:
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
                                  //  break;
                                }
                               // else
  //                              if((c->getDtype() == BanDComponentType::bAtom) && (*c == *dataList.at(i)))
  //                                  copy->dataList.append(c);

                           }
                        }
                   }
                    else{
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
        break;
    }
    case BanSComponentType::bStatement:
    case BanSComponentType::bSOperator:
        break;
    }
//    for(int i=0; i<dataList.count();i++)
//    {
//        copy->dataList.insert(i,dataList.at(i));
//    }
    //copy->dataList=dataList;
    copy->dataID=dataID;
    copy->unifies=unifies;
    copy->printStack=printStack;
    //copy->dataList=dataList;

    return copy;
}

void BanDataList::instantiateObject(BanSComponent *Scomp)
{
    BanDataList *dataL=dynamic_cast<BanDataList*>(Scomp);
    int length1=this->dataList.size();
    int length2=dataL->dataList.size();
    if(length1==length2)
    {
        switch(this->getStype())
        {
        case BanSComponentType::bData:
        {
            for(int i=0; i<length1; i++)
            {
                switch (this->dataList.at(i)->getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    BanDAtom *atom1=dynamic_cast<BanDAtom*>(this->dataList.at(i));
                    BanDAtom *atom2=dynamic_cast<BanDAtom*>(dataL->dataList.at(i));
                    atom1->instantiateObject(atom2);

                    break;
                }
                case BanDComponentType::bOperator:
                {
                    BanDOperator *op1=dynamic_cast<BanDOperator*>(this->dataList.at(i));
                    BanDOperator *op2=dynamic_cast<BanDOperator*>(dataL->dataList.at(i));
                    op1->setId(op2->getID());
                    op1->setAdtype(op2->getDOtype());
                    op2->setInstantiate(op2->getInstantiate());
                    break;
                }
                case BanDComponentType::bAnyData:
                {
                    BanDataC *dc=dynamic_cast<BanDataC*>(this->dataList.at(i));
                    dc->instantiateObject(dataL->dataList.at(i));
                    break;
                }

                }
            }
            break;
        }
        case BanSComponentType::bSOperator:
        {
            break;
        }
        case BanSComponentType::bStatement:
        {
            break;
        }
        }
    }
    else
    {
        if(Scomp->getStype()==BanSComponentType::bData){
            int holder=0;

            switch(this->getStype())
            {
            case BanSComponentType::bData:
            {

                for(int i=0; i<length1; i++)
                {
                    switch(this->dataList.at(i)->getDtype())
                    {
                    case BanDComponentType::bAnyData:
                    {
                        BanDataC *dc=dynamic_cast<BanDataC*>(this->dataList.at(i));
                        this->dataList.pop_front();
                        this->printStack.pop();
                        for(int j=0; j<length2; j++)
                        {
                            holder=j;
                            dataList.append(dataL->dataList.at(j));
                            printStack.append(dataL->dataList.at(j)->getID());
                        }
                        break;
                    }
                    case BanDComponentType::bAtom:
                    {
                        BanDAtom *atom1=dynamic_cast<BanDAtom*>(this->dataList.at(i));
                        for(int j=holder; j<length2; j++)
                        {
                            atom1->unify(*dataL->dataList.at(j));

                        }
                        break;
                    }
                    case BanDComponentType::bOperator:
                    {
                        BanDOperator *op1=dynamic_cast<BanDOperator*>(this->dataList.at(i));
                        for(int j=holder; j<length2; j++)
                        {
                            BanDOperator *op2=dynamic_cast<BanDOperator*>(dataL->dataList.at(j));
                            op1->setId(op2->getID());
                            op1->setAdtype(op2->getDOtype());
                            op1->setInstantiate(op2->getInstantiate());
                        }
                        break;
                    }

                    }
                }

                break;
            }
            case BanSComponentType::bSOperator:
            {
                break;
            }
            case BanSComponentType::bStatement:
            {
                break;
            }

            }
        }

    }
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
        //  dataList=orig.dataList;
        // setId(orig.getID());
        printStack=orig.printStack;
        instantiate= orig.getInstantiate();
        stype=orig.stype;
        foreach(BanDComponent *ptr,orig.dataList)
        {
            switch(ptr->getDtype())
            {
            case BanDComponentType::bAtom:
            {
                BanDAtom *d1=dynamic_cast<BanDAtom *>(ptr);
                a1=new BanDAtom();
                a1->setId(d1->getID());
                a1->setAtomtype(d1->getAtype());
                a1->setInverseKey(d1->getInverseKey());
                a1->setInstantiate(d1->getInstantiate());
                dataList.append(a1);


                break;
            }
            case BanDComponentType::bOperator:
            {
                BanDOperator *d1=dynamic_cast<BanDOperator *>(ptr);
                op1=new BanDOperator(d1->getDOtype());
                op1->setId(d1->getID());
                op1->setInstantiate(d1->getInstantiate());
                dataList.append(op1);

                break;
            }
            case BanDComponentType::bAnyData:
            {
                BanDataC *d1=dynamic_cast<BanDataC *>(ptr);
                dc1=new BanDataC();
                // dc1->setId(d1->getID());
                dc1->setInstantiate(d1->getInstantiate());
                break;
            }
            }

        }
        break;
    }

    }


}

BanDataList::~BanDataList()
{

}

bool BANLogic::BanDataList::match(BanSComponent &Scomp)
{
    int dataTypeIndex=0;
    ifMatches=false;
    int count=0,counter=0;
    if(Scomp.getStype()==BanSComponentType::bData){
        BanDataList *data=dynamic_cast<BanDataList*>(&Scomp);
        if(this->dataList.count()==data->dataList.count())
        {
            for(int k=this->dataList.count()-1; k>=0; k--)
            {
                switch(this->dataList.at(k)->getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    BanDAtom *atom1=dynamic_cast<BanDAtom*>(this->dataList.value(k));
                    BanDAtom *atom2=dynamic_cast<BanDAtom*>(data->dataList.value(k));

                    if(atom1->match(*atom2))
                    {
                        ifMatches=true;
                        counter++;
                    }
                    else
                    {
                        ifMatches=false;
                        break;
                    }
                    break;
                }
                case BanDComponentType::bOperator:
                {
                    if(this->dataList.value(k)->match(*data->dataList.value(k)))
                    {

                        ifMatches=true;
                        counter++;
                    }
                    else
                    {
                        ifMatches=false;
                        break;
                    }
                    break;
                }
                case BanDComponentType::bAnyData:{
                    dataTypeIndex=k;
                    BanDataC *dataany1=dynamic_cast<BanDataC *>(this->dataList.value(k));
                    if(dataany1->match(*data->dataList.value(k)))
                    {
                        ifMatches=true;
                        counter++;
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
            if(counter==dataList.size())
            {
                ifMatches=true;
            }
            else {
                ifMatches=false;
            }
        }
        else if(this->dataList.count()!=data->dataList.count()){
            switch(Scomp.getStype())
            {
            case BanSComponentType::bData:
            {
                BanDataList *data=dynamic_cast<BanDataList*>(&Scomp);
                int i, j=data->dataList.count()-1,k=0,index=0;
                for(i=dataList.count()-1; i>=0; i--)
                {
                    switch(dataList.value(i)->getDtype())
                    {
                    case BanDComponentType::bAtom:
                    {
                        if(dataList.value(i)->match(*data->dataList.value(j)))
                        {
                            ifMatches=true;
                            count++;
                            j--;
                        }
                        else
                        {
                            j--;
                            ifMatches=false;
                            break;
                        }
                        break;
                    }
                    case BanDComponentType::bOperator:
                    {
                        if(data->dataList.value(j)->getDtype()==BanDComponentType::bOperator)
                        {
                            BanDOperator *ref= dynamic_cast<BanDOperator*>(data->dataList.value(j));
                            if(dataList.value(i)->match(*ref))
                            {
                                j--;
                                ifMatches=true;
                                count++;
                            }
                            else
                            {
                                j--;
                                ifMatches=false;
                                break;
                            }
                        }
                        else ifMatches=false;
                        break;
                    }
                    case BanDComponentType::bAnyData:
                    {
                        index=i;
                        for(k=j;k>=0; k--)
                        {
                            if((dataList.value(i)->match(*data->dataList.value(k))))
                            {
                                switch(data->dataList.value(k)->getDtype())
                                {
                                case BanDComponentType::bOperator:{
                                    BanDOperator *o=dynamic_cast<BanDOperator*>(data->dataList.value(k));
                                    if(dataList.value(i)->match(*o))
                                    {
                                        j--;
                                        ifMatches=true;
                                        count++;
                                    }
                                    else
                                    {
                                        j--;
                                        ifMatches=false;
                                        break;
                                    }
                                    break;
                                }
                                case BanDComponentType::bAtom:
                                {
                                    BanDAtom *a=dynamic_cast<BanDAtom*>(data->dataList.value(k));
                                    if(dataList.value(i)->match(*a))
                                    {
                                        ifMatches=true;
                                        count++;
                                        j--;
                                    }
                                    else
                                    {
                                        j--;
                                        ifMatches=false;
                                        break;
                                    }
                                    break;
                                }
                                }
                            }
                        }
                    }
                    }
                }
                if(count==data->dataList.count())
                {
                    ifMatches=true;
                }
                else
                    ifMatches=false;
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

        }


    }
    else ifMatches=false;

    return ifMatches;
}

bool BANLogic::BanDataList::unify(BanSComponent &Scomp)
{
    QList<BanDComponent*> inst;
    QStack<QString> stringRep;
    int dataTypeIndex=0;
    QStack<BanDOperator*> opStack;
    if(this->match(Scomp))
    {
        BanDataList *data=dynamic_cast<BanDataList*>(&Scomp);
        if(dataList.count()==data->dataList.count())
        {
            for(int k=dataList.count()-1; k>=0; k--)
            {
                switch(dataList.at(k)->getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    BanDAtom *atom1=dynamic_cast<BanDAtom*>(dataList.value(k));
                    BanDAtom *atom2=dynamic_cast<BanDAtom*>(data->dataList.value(k));

                    if(atom1->unify(*atom2))
                    {
                        dataList.replace(k,data->dataList.value(k));
                        dataList.at(k)->setId(data->dataList.value(k)->getID());

                        // this->printStack.append(data->dataList.value(k)->getID());
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
                    if(this->dataList.value(k)->unify(*data->dataList.value(k)))
                    {
                        this->dataList.replace(k,data->dataList.value(k));
                        //this->printStack.append(data->dataList.value(k)->getID());

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
                    dataTypeIndex=k;
                    BanDataC *dataany1=dynamic_cast<BanDataC *>(dataList.value(k));
                    if(dataany1->unify(*data->dataList.value(k)))
                    {
                        dataList.removeAt(k);

                        switch(data->dataList.value(k)->getDtype())
                        {
                        case BanDComponentType::bOperator:{
                            BanDOperator *o=dynamic_cast<BanDOperator*>(data->dataList.value(k));
                            dataList.insert(k,o);
                            dataList.at(k)->setId(o->getID());
                            break;
                        }
                        case BanDComponentType::bAtom:
                        {
                            BanDAtom *a=dynamic_cast<BanDAtom*>(data->dataList.value(k));
                            dataList.insert(k,a);
                            dataList.value(k)->setInstantiate(true);
                            dataList.at(k)->setId(a->getID());

                            //this->dataList.at(k)->setId(data->dataList.value(k)->getID());

                            break;
                        }
                        case BanDComponentType::bAnyData:
                        {
                            unifies=false;
                            break;
                        }
                        }
                        break;
                    }
                }
                }
            }
        }
        else
        {
            int i,j=data->dataList.count()-1, k=0,count=0,index=0, anydataindexCount=0;
            for(i=dataList.count()-1; i>=0; i--)
            {
                switch(dataList.value(i)->getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    if(data->dataList.value(j)->getDtype()==BanDComponentType::bAtom){
                        if(dataList.value(i)->unify(*data->dataList.value(j)))
                        {
                            //this->dataList.at(i)->setId(data->dataList.value(j)->getID());
                            dataList.replace(i,data->dataList.value(j));
                            printStack.replace(i,data->dataList.value(j)->getID());
                            j--;
                            unifies=true;
                            count++;
                        }
                        else
                        {
                            j--;
                            unifies=false;
                            break;
                        }
                    }
                    else unifies=false;
                    break;
                }
                case BanDComponentType::bOperator:
                {
                    if(data->dataList.value(j)->getDtype()==BanDComponentType::bOperator){

                        BanDOperator *ref= dynamic_cast<BanDOperator*>(data->dataList.value(j));
                        if(dataList.value(i)->unify(*ref))
                        {
                            dataList.replace(i,data->dataList.value(j));
                            this->printStack.append(data->dataList.value(j)->getID());
                            j--;
                            unifies=true;
                            count++;
                        }
                        else
                        {
                            j--;
                            unifies=false;
                            break;
                        }
                    }else unifies=false;
                    break;
                }
                case BanDComponentType::bAnyData:
                {
                    index=i;
                    anydataindexCount++;
                    for(k=j;k>=0; k--)
                    {
                        switch(data->dataList.value(k)->getDtype())
                        {
                        case BanDComponentType::bOperator:{
                            BanDOperator *o=dynamic_cast<BanDOperator*>(data->dataList.value(k));
                            opStack.append(o);
                            j--;
                            break;
                        }
                        case BanDComponentType::bAtom:
                        {
                            BanDAtom *a=dynamic_cast<BanDAtom*>(data->dataList.value(k));
                            otherPart.append(a);
                            j--;
                            break;
                        }
                        case BanDComponentType::bAnyData:
                        {
                            j--;
                            unifies=false;
                            break;
                        }
                        }
                    }
                    if(!opStack.isEmpty())
                    {
                        unifies=instantiateAnyData(otherPart, opStack, index);
                    }
                    break;
                }
                }
            }
        }
        if(unifies)
        {
            BanDataList *d1=new BanDataList(dataList);
            //this->printStack=d1->printStack;
            //this->setId(printStack.top());
        }
    }
    return unifies;

}

bool BanDataList::instantiateAnyData(QStack<BanDComponent *> otherPart, QStack<BanDOperator *> op, int index)
{   //              0  1 2
    //anyD has     Kab B A
    //op has    shareKey
    bool found =false;
    QList<BanDComponent*> Y;
    QList<BanDComponent*> X;
    if(!op.isEmpty()){
        for(int i=0; i<op.count(); i++)
        {
            BanDOperator *myop=dynamic_cast<BanDOperator*>(op.at(i));
            switch(myop->getDOtype())
            {
            case BanDOperatorType::ShareKey:
            {
                op.pop();
                Y.append(otherPart.pop());
                Y.append(otherPart.pop());
                Y.append(otherPart.pop());
                break;
            }
            case BanDOperatorType::concates:
            {
                op.pop();
                Y.append(otherPart.pop());
                Y.append(otherPart.pop());
                break;
            }
            case BanDOperatorType::Encryption:{
                op.pop();
                Y.append(otherPart.pop());
                Y.append(otherPart.pop());
                break;
            }
            case BanDOperatorType::FreshData:{
                op.pop();
                Y.append(otherPart.pop());
                //this->dataList.append(myOperator);
                break;
            }
            case BanDOperatorType::ShareSecret:{
                op.pop();
                Y.append(otherPart.pop());
                Y.append(otherPart.pop());
                Y.append(otherPart.pop());;
                break;
            }
            case BanDOperatorType::HasKey:{
                op.pop();
                Y.append(otherPart.pop());
                Y.append(otherPart.pop());
                break;
            }
            case BanDOperatorType::SecretPassword:{
                op.pop();
                Y.append(otherPart.pop());
                Y.append(otherPart.pop());
                break;
            }
            }
        }

    }
    else if(op.isEmpty())
    {
        if(!otherPart.isEmpty())
        {
            for(int k=0; k<otherPart.count(); k++)
                X.append(otherPart.pop());
        }

        QString s="";
        if(!Y.isEmpty()){
            foreach(BanDComponent *ptr,Y)
            {
                s.clear();
                if(   this->dataList.at(index)->getDtype()==BanDComponentType::bAnyData)
                {
                    this->dataList.replace(index,ptr);
                    //s.append(ptr->getID()+" ");

                    // this->printStack.replace(index,ptr->getID());
                }
                else
                {
                    this->dataList.append(ptr);
                }
            }
        }
        else
            foreach(BanDComponent *ptr,X)
            {
                s.clear();
                if(this->dataList.at(index)->getDtype()==BanDComponentType::bAnyData)
                {
                    this->dataList.replace(index,ptr);

                }
                else
                {
                    this->dataList.append(ptr);

                }
            }
        found=true;
    }
    else
    {
        found = false;
    }
    return found;
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
    //        foreach(BanDComponent *str, this->dataList)
    //        {
    //            s=str->getString();
    //        }

    //        BanDataList *d=new BanDataList(this->dataList);
    //            foreach(BanDComponent *ptr, d->getDataList())
    //                s.append(ptr->getString());
    foreach(BanDComponent *ptr,this->dataList)
    {
        switch(ptr->getDtype())
        {
        case BanDComponentType::bAtom:
        {
            s.clear();

            BanDAtom *d1=dynamic_cast<BanDAtom *>(ptr);
            s.append(d1->getString());
            break;
        }
        case BanDComponentType::bOperator:
        {
            s.clear();

            BanDOperator *d1=dynamic_cast<BanDOperator *>(ptr);
            s.append(d1->getString());
            break;
        }
        case BanDComponentType::bAnyData:
        {
            s.clear();

            BanDataC *d1=dynamic_cast<BanDataC *>(ptr);
            s.append(d1->getString());
            break;
        }
        }

        //        QTextStream(stdout)<<ptr->getID();
    }

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
