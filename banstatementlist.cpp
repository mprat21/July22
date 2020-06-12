#include "banstatementlist.h"
using namespace BANLogic;
QList<BanSComponent *> BanStatementList::getStList() const
{
    return stList;
}

QStack<QString> BanStatementList::getPrintStStack() const
{
    return printStStack;
}

BANLogic::BanStatementList::BanStatementList()
{
}
BANLogic::BanStatementList::BanStatementList(QList<BanSComponent *> sList):BanSComponent(BanSComponentType::bStatement)
{
    this->stList=sList;
    foreach(BanSComponent *ptr, this->stList)
    {
        if(ptr->getStype()==BanSComponentType::bData)
        {
            BanDataList *comp1=dynamic_cast<BanDataList *>(ptr);
            // BanDataList *newpt=new BanDataList(comp1->getDataList());
            foreach(QString ptr, comp1->getPrintStack())
            {
                this->printStStack.push(ptr);
            }
        }
        else if(ptr->getStype()==BanSComponentType::bSOperator)
        {
            banSOperator *comp1=dynamic_cast<banSOperator *>(ptr);
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
        }
        else if(ptr->getStype()==BanSComponentType::bStatement)
        {
            break;

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
            banSOperator *d1=dynamic_cast<banSOperator *>(ptr);
            d1->printRPN();
            break;
        }
        case BanSComponentType::bStatement:
        {
            BanStatementList *d1=dynamic_cast<BanStatementList *>(ptr);
            d1->printRPN();
            break;
        }
        }
        //QTextStream(stdout)<<ptr->getID();
    }
    QTextStream(stdout)<<"}";
    cout<<endl<<endl;
}

bool BANLogic::BanStatementList::match(BanSComponent &value)
{
    return false;
}

bool BANLogic::BanStatementList::unify(BanSComponent &value)
{
    return false;
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
BanStatementList *St=new BanStatementList(this->getStList());
    foreach(QString ptr, St->getPrintStStack())
    {
        QTextStream(stdout)<<ptr;
    }

}
