#ifndef BANPOSTULATES_H
#define BANPOSTULATES_H
#include "banstatementlist.h"
#include "../LPT/Postulate.h"

namespace BANLogic{
class BanPostulates: public LPT::Postulate
{
private:
    QString rule;
    BanRuleId id;
    BanStatementList *goals;
    QList<BanStatementList*> prereq,temp;
    QList<BanSComponent*> pComponentList;
    QList<BanPostulates*> allPostulate;
    LPT::LPTPtrList<BanDComponent> allAtomicComponents;


public:
    BanPostulates();
    BanPostulates(QString ruleName, BanStatementList *g, QList<BanStatementList*> prerequisites);
    BanPostulates(BanRuleId ruleid, LPT::Statement *pGoal, LPT::StatementPtrList &prereq);
    BanPostulates(BanPostulates &orig);
    QString getRule() const;
    //QList<BanStatementList *> getBanPrerequisites(BanRuleId rule) const;
    QList<BanSComponent*> getPComponentList() const;
    void print();
    void printRPN();
    inline BanRuleId getRuleID() {return id;}
    inline BanStatementList *getBanGoal() {return dynamic_cast<BanStatementList *>(goal);}
    void updateAtomicComponents();


    // Postulate interface
    virtual LPT::Postulate *getCopy() override;
    virtual QString getString() override;
    virtual bool operator ==(LPT::Postulate &p2) override;
};
}
#endif // BANPOSTULATES_H

