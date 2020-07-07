#ifndef BANPOSTULATES_H
#define BANPOSTULATES_H
#include "banstatementlist.h"

namespace BANLogic{
class BanPostulates
{
private:
    QString rule;
    BanStatementList *goal;
    QList<BanStatementList*> prerequisites;
    QList<BanStatementList*> pComponentList;

public:
    BanPostulates();
    BanPostulates(QString ruleName, BanStatementList *g, QList<BanStatementList*> prerequisites);
    QString getRule() const;
    BanStatementList *getGoal() const;
    QList<BanStatementList *> getPrerequisites() const;
    QList<BanStatementList *> getPComponentList() const;
    void print();
    void printRPN();

};

#endif // BANPOSTULATES_H
}
