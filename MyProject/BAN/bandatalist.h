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

public:
    BanDataList();
    BanDataList(QList<BanDComponent*> dList1);
    BanDataList(BanDataList &orig);
    virtual void printRPN() override;
    virtual void print() override;
    virtual  QString  getID() override{return dataID;}
    QList<BanDComponent *> getDataList();
    QStack<QString> getPrintStack();
    virtual bool match(BanSComponent &Scomp) override;
    virtual bool unify(BanSComponent &Scomp) override;
    bool getInstantiate() const override;
    bool getIfMatches() const override;
    QList<BanSComponent *> getStList() const;
   // QList<BanDataList *> getDList1() const;

    virtual void setId(const QString &value) override;

    // BanSComponent interface
public:
    virtual QString getString() override;
    virtual bool operator ==(const BanSComponent &other) override;
};
}
#endif // BANDATALIST_H