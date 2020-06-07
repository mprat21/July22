#ifndef BANDATALIST_H
#define BANDATALIST_H
#include "bandcomponent.h"
#include "banscomponent.h"
#include "bandoperator.h"
#include "bandatom.h"


namespace BANLogic {

class BanDataList : public BanSComponent
{
protected:
   static int datacount;
    QString dataID;
    QList<BanDComponent*> dataList;
    QStack<QString> printStack;
    QList<BanSComponent*> StList;


public:
    BanDataList();
    BanDataList( QList<BanDComponent*> dList);
    BanDataList(QList<BanSComponent*> dList);

    virtual void printRPN();
    virtual void print() override;
    virtual QString getID() override{return dataID;}
    QList<BanDComponent *> getDataList();
    QStack<QString> getPrintStack();
    virtual bool match(BanSComponent &Scomp) override;
    virtual bool unify(BanSComponent &Scomp) override;
    bool getInstantiate() const override;
    bool getIfMatches() const override;
};
}
#endif // BANDATALIST_H
