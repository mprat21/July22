#include "banpostulates.h"

using namespace BANLogic;
namespace BANLogic {

QString BanPostulates::getRule() const
{
    return rule;
}

BanStatementList *BanPostulates::getBanGoal() const
{
    return goal;
}

QList<BanStatementList *> BanPostulates::getBanPrerequisites(QString r) const
{
    QList<BanStatementList*> preReq;
    preReq.clear();
    foreach(BanPostulates *pos, allPostulate)
    {
        if(pos->rule==r)
            preReq.append(pos->temp);
    }
    return preReq;
}

QList<BanSComponent *> BanPostulates::getPComponentList() const
{
    return pComponentList;
}

void BanPostulates::print()
{
    QList<BanStatementList *> preList;
    QTextStream(stdout)<< "Postulate ID: " << rule;
    cout << endl << "Pre:\n";
    preList=getBanPrerequisites(rule);
    foreach(BanStatementList *pre,preList)
    {
        pre->print();
        cout << endl;
    }
    QTextStream(stdout) << "\nGoal:\n";goal->print();
    cout << endl;

}

void BanPostulates::printRPN()
{
    QList<BanStatementList *> preList;
    QTextStream(stdout)<< "Postulate ID: " << rule;
    cout << endl << "Pre:\n";
    preList=getBanPrerequisites(rule);
    foreach(BanStatementList *pre,preList)
    {
        pre->printRPN();
        cout << endl;
    }
    QTextStream(stdout) << "\nGoal:\n";goal->printRPN();
    cout << endl;

}

BanPostulates::BanPostulates()
{

}

BanPostulates::BanPostulates(QString ruleName, BanStatementList *g, QList<BanStatementList *> preList)
{
    temp.clear();
    BanStatementList *origPre, *newPre;
    rule=ruleName;
    // create copy of goal
    goal = new BanStatementList(*g);
    if (goal == NULL) {
        throw new BanException("Failed to allocate new goal in banPostulate Constructor");
    }
    // copy all prerequisites
    foreach(origPre, preList)
    {
        newPre = new BanStatementList(*origPre);
        if (newPre == NULL) {
            throw new BanException("Failed to allocate newPre in banPostulate Constructor");
        }
        prerequisites.append(newPre);
        temp.append(newPre);

    }
    allPostulate.append(this);
}

BanPostulates::BanPostulates(BanPostulates &orig):rule(orig.rule)
{
    temp.clear();
    BanStatementList *origPre, *newPre;
    rule=orig.rule;

    // create copy of goal
    goal = new BanStatementList(*orig.goal);
    if (goal == NULL) {
        throw new BanException("Failed to allocate new Data goal in banPostulate Copy Constructor");
    }
    // copy all prerequisites
    foreach(origPre, orig.prerequisites)
    {
        newPre = new BanStatementList(*origPre);
        if (newPre == NULL) {
            throw new BanException("Failed to allocate new Data newPre in banPostulate Copy Constructor");
        }
        prerequisites.append(newPre);
        temp.append(newPre);
    }
    allPostulate.append(this);
}

}


LPT::Postulate *BANLogic::BanPostulates::getCopy()
{
}

QString BANLogic::BanPostulates::getString()
{
}

bool BANLogic::BanPostulates::operator ==(LPT::Postulate &p2)
{
}
