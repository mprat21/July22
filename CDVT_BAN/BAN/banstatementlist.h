#ifndef BANSTATEMENTLIST_H
#define BANSTATEMENTLIST_H
#include "../LPT/LPTExceptions.h"
#include "../LPT/LPTDefinitions.h"
#include "../LPT/Statement.h"
#include "banscomponent.h"
#include "bansoperator.h"
#include "bandatalist.h"
#include "bandatom.h"

#include <LPT/LPTList.h>

namespace BANLogic{

class BanStatementList : public BanSComponent, public LPT::Statement
{
protected:
    static int statementCount;
   // BanSOperatorType stype;
    QList<BanSComponent*> stList;
    QStack<QString> printStStack;
    QString stid;

public:
    BanStatementList();
    BanStatementList(QList<BanSComponent*> stList);
    BanStatementList(BanStatementList &orig);
    //BanStatementList(BanStatementList &orig, QList<BanSComponent*> cList);
    // BanSComponent interface
    virtual void print() override;
    virtual void printRPN() override;
    virtual QString getID()override{return stid;}
    bool match(BanSComponent &value)override;
    bool unify(BanSComponent &value)override;
    bool getInstantiate() const override;
    bool getIfMatches() const override;
    QList<BanSComponent *> getStList() const;
    QStack<QString> getPrintStStack() const;
    virtual void setId(const QString &value) override;

    // Statement interface
    virtual bool operator ==(const LPT::Statement &stmt) override;
    virtual QString getString() override;
    // BanSComponent interface
    virtual bool operator ==(const BanSComponent &other) override;
    void setStList(const QList<BanSComponent *> &value);

    void findData(QList<BanDComponent*> sList) const;
    BanStatementList* getCopy(LPT::LPTPtrList<BanDComponent> &components);

};

}
#endif // BANSTATEMENTLIST_H
