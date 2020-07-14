#ifndef BANPOSTULATES_H
#define BANPOSTULATES_H
#include "banstatementlist.h"
#include "../LPT/Postulate.h"

namespace BANLogic{
class BanPostulates: public LPT::Postulate
{
private:
    QString rule;
    BanStatementList *goal;
    QList<BanStatementList*> prerequisites,temp;
    QList<BanSComponent*> pComponentList;
    QList<BanPostulates*> allPostulate;

public:
    BanPostulates();
    BanPostulates(QString ruleName, BanStatementList *g, QList<BanStatementList*> prerequisites);
    BanPostulates(BanPostulates &orig);
    QString getRule() const;
    BanStatementList *getBanGoal() const;
    QList<BanStatementList *> getBanPrerequisites(QString rule) const;
    QList<BanSComponent*> getPComponentList() const;
    void print();
    void printRPN();

    // Postulate interface
public:
    virtual LPT::Postulate *getCopy() override;
    virtual QString getString() override;
    virtual bool operator ==(LPT::Postulate &p2) override;
};
}
#endif // BANPOSTULATES_H

