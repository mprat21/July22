#include "bandatac.h"
int BANLogic::BanDataC::dataCount=1;

QStack<QString> BANLogic::BanDataC::getPrintQStack() const
{
    return printQStack;
}

BANLogic::BanDataC::BanDataC():BanDComponent(BanDComponentType::bAnyData)
{
    if(this->dataValue=="" || this->dataValue.isEmpty() || this->dataValue.isNull())
    {
        this->dataValue=QString("_D_%1").arg(dataCount++);
        this->type=BanDComponentType::bAnyData;
        this->instantiate=false;
        myListdata.append(this);
    }
}

BANLogic::BanDataC::BanDataC(BanDComponentType type, QList<BANLogic::BanDComponent *> lists):BanDComponent(BanDComponentType::bAnyData)
{
    this->myListdata=lists;
         if(this->myListdata.size()>1)
         {
             printQStack.push(this->getID());
                 BanDataList *dataList1=new BanDataList(this->myListdata);

         }
}

void BANLogic::BanDataC::setDtype(QString dType)
{
}

void BANLogic::BanDataC::setId(const QString &value)
{
    this->dataValue=value;
}

QString BANLogic::BanDataC::getID()
{
    return this->dataValue;
}


void BANLogic::BanDataC::print()
{
    foreach(BanDComponent *ptr,this->myListdata)
    {
         ptr->print();
    }
}

bool BANLogic::BanDataC::getInstantiate() const
{
    return instantiate;
}

bool BANLogic::BanDataC::match(BANLogic::BanDComponent *value)
{
    return false;
}


bool BANLogic::BanDataC::unify(BANLogic::BanDComponent *value)
{
    return false;
}

bool BANLogic::BanDataC::getIfMatches() const
{
    return ifMatches;
}

QList<BANLogic::BanDComponent *> BANLogic::BanDataC::getMyListdata() const
{
    return myListdata;
}

void BANLogic::BanDataC::printRPN()
{
    foreach(BanDComponent *ptr, this->getMyListdata())
    {
        QTextStream(stdout)<<ptr->getID()<<" ";
    }
}
