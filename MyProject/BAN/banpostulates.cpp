#include "banpostulates.h"

using namespace BANLogic;
namespace BANLogic {

QString BanPostulates::getRule() const
{
    return rule;
}

QList<BanStatementList *> BanPostulates::getBanPrerequisites(BanRuleId r) const
{
    QList<BanStatementList*> preReq;
    preReq.clear();
    foreach(BanPostulates *pos, allPostulate)
    {
        if(pos->getRuleID()==r)
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
    preList=getBanPrerequisites(getRuleID());
    foreach(BanStatementList *pre,preList)
    {
        pre->print();
        cout << endl;
    }
    QTextStream(stdout) << "\nGoal:\n";goals->print();
    cout << endl;

}

void BanPostulates::printRPN()
{
    QList<BanStatementList *> preList;
    QTextStream(stdout)<< "Postulate ID: " << rule;
    cout << endl << "Pre:\n";
    preList=getBanPrerequisites(getRuleID());
    foreach(BanStatementList *pre,preList)
    {
        pre->printRPN();
        cout << endl;
    }
    QTextStream(stdout) << "\nGoal:\n";goals->printRPN();
    cout << endl;

}

BanPostulates::BanPostulates()
{

}

BanPostulates::BanPostulates(QString ruleName, BanStatementList *g, QList<BanStatementList *> preList)
{
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
        prereq.append(newPre);
    }
    allPostulate.append(this);
}

BanPostulates::BanPostulates(BanRuleId ruleid, LPT::Statement *pGoal, LPT::StatementPtrList &prereq):id(ruleid)
{
    goal = pGoal;
    prerequisites.append(prereq);
    postulateName = BanRuleNames[static_cast<int>(id)];
    rule = QString("(");
    for (int i=0; i<prerequisites.count(); ++i) {
        rule.append(prerequisites.at(i)->getString());
        if (i < prerequisites.count()-1) rule.append(", ");
    }
    rule.append(") -> (" + goal->getString() + ")");
    //updateAtomicComponents();
}

BanPostulates::BanPostulates(BanPostulates &orig):rule(orig.rule)
{
    temp.clear();
    BanStatementList *origPre, *newPre;
    rule=orig.rule;

    // create copy of goal
    goals = new BanStatementList(*orig.goals);
    if (goals == NULL) {
        throw new BanException("Failed to allocate new Data goal in banPostulate Copy Constructor");
    }
    // copy all prerequisites
    foreach(origPre, orig.prereq)
    {
        newPre = new BanStatementList(*origPre);
        if (newPre == NULL) {
            throw new BanException("Failed to allocate new Data newPre in banPostulate Copy Constructor");
        }
        prereq.append(newPre);
        temp.append(newPre);
    }
    allPostulate.append(this);
}

LPT::Postulate *BANLogic::BanPostulates::getCopy()
{
}

QString BANLogic::BanPostulates::getString()
{
}

bool BANLogic::BanPostulates::operator ==(LPT::Postulate &p2)
{
    return true;
}
}
