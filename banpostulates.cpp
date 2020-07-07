#include "banpostulates.h"

using namespace BANLogic;

QString BanPostulates::getRule() const
{
    return rule;
}

BanStatementList *BanPostulates::getGoal() const
{
    return goal;
}

QList<BanStatementList *> BanPostulates::getPrerequisites() const
{
    return prerequisites;
}

QList<BanStatementList *> BanPostulates::getPComponentList() const
{
    return pComponentList;
}

void BanPostulates::print()
{

}

void BanPostulates::printRPN()
{

}

BanPostulates::BanPostulates()
{

}

BanPostulates::BanPostulates(QString ruleName, BanStatementList *g, QList<BanStatementList *> prerequisites)
{

}
