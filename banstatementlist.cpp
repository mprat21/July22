#include "banstatementlist.h"
using namespace BANLogic;
BANLogic::BanStatementList::BanStatementList()
{
}
BANLogic::BanStatementList::BanStatementList(QList<BanSComponent *> sList):BanSComponent(BanSComponentType::bStatement)
{
    stList=sList;
    foreach(BanSComponent *ptr, stList)
    {
        if(ptr->getStype()==BanSComponentType::bData)
        {
            BanDataList *comp1=dynamic_cast<BanDataList *>(ptr);
            BanDataList *newpt=new BanDataList(comp1->getDataList());
            foreach(QString ptr, newpt->getPrintStack())
            {
                printStStack.push(ptr);
            }
        }
        else if(ptr->getStype()==BanSComponentType::bSOperator)
        {
            banSOperator *comp1=dynamic_cast<banSOperator *>(ptr);
            switch (comp1->getStOptype())
            {
            case BanSOperatorType::told:{
                printStStack.push(printStStack.pop()+comp1->getID()+printStStack.pop());
                break;
            }
            case BanSOperatorType::believe:{
                printStStack.push(printStStack.pop()+comp1->getID()+printStStack.pop());
                break;
            }
            case BanSOperatorType::possess:{
                printStStack.push(printStStack.pop()+comp1->getID()+printStStack.pop());
                break;
            }
            case BanSOperatorType::conveyed:{
                printStStack.push(printStStack.pop()+comp1->getID()+printStStack.pop());
                break;
            }
            case BanSOperatorType::hasJurisdiction:{
                printStStack.push(printStStack.pop()+comp1->getID()+printStStack.pop());
                break;
            }
            }
        }
        else if(ptr->getStype()==BanSComponentType::bStatement)
        {
            BanSComponent *comp=new BanStatementList(stList);
        }
    }
}
void BANLogic::BanStatementList::printRPN()
{
    QTextStream(stdout)<<"{";
    foreach(BanSComponent *ptr, stList)
    {
        if(ptr->getStype()==BanSComponentType::bData)
        {
            BanDataList *comp1=dynamic_cast<BanDataList *>(ptr);
            comp1->printRPN();
        }
        else
        {
            ptr->print();
        }
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
    foreach(QString ptr, printStStack)
    {
        QTextStream(stdout)<<ptr;
    }
    cout<<endl<<endl;
}
