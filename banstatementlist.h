#ifndef BANSTATEMENTLIST_H
#define BANSTATEMENTLIST_H
#include "banscomponent.h"
#include "bansoperator.h"
#include "bandatalist.h"
#include "bandatom.h"

namespace BANLogic{

class BanStatementList : public BanSComponent
{
protected:
    BanSOperatorType stype;
    QList<BanSComponent*> stList;
    QStack<QString> printStStack;
    QString stid;

public:
    BanStatementList();
    BanStatementList(QList<BanSComponent*> stList);

    // BanSComponent interface
    virtual void print() override;
    virtual void printRPN() override;
    virtual QString getID()override{return stid;}
    bool match(BanSComponent &value)override;
    bool unify(BanSComponent &value)override;
    bool getInstantiate() const override;
    bool getIfMatches() const override;
    QList<BanSComponent *> getStList() const;
};

}
#endif // BANSTATEMENTLIST_H
