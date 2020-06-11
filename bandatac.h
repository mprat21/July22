#ifndef BANDATAC_H
#define BANDATAC_H
#include "bandatalist.h"
#include "bandcomponent.h"

namespace BANLogic{
class BanDataC:public BanDComponent
{
protected:
    static int dataCount;
    BanDComponentType type;
    QString dataValue;
    QList<BanDComponent*> myListdata;
    QStack<QString> printQStack;
    BanDataList *dList;


public:
    BanDataC();
   // BanDataC(QList<BanDComponent*> dList1);
    virtual void setDtype(QString dType) override;
    virtual void setId(const QString &value) override;
    virtual QString getID() override;
    virtual void print() override;
    virtual bool getInstantiate() const override;
    virtual bool match(BanDComponent *value) override;
    virtual bool unify(BanDComponent *value) override;
    virtual bool getIfMatches() const override;
    QList<BanDComponent *> getMyListdata() const;
    virtual void printRPN()override;
    QStack<QString> getPrintQStack() const;
    virtual void setInstantiate(bool value) override{this->instantiate=value;}

};
}
#endif // BANDATAC_H
