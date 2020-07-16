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
    //cout<<rule.toStdString()<<endl;
}

BanPostulates::BanPostulates(BanPostulates &orig):rule(orig.rule)
{
  BanStatementList *pr, *go;
  id=orig.getRuleID();
  rule=orig.rule;

  QList<BanSComponent*> goalList=orig.getBanGoal()->getStList();
  go=new BanStatementList(goalList);
  goal=go;

  QList<BanStatementList*> preList=orig.getBanPrerequisites(id);
  for (int i=0; i<preList.count(); ++i) {
      prerequisites.append(preList.value(i));
  }
}

LPT::Postulate *BANLogic::BanPostulates::getCopy()
{
    return new BanPostulates(*this);
}

QString BANLogic::BanPostulates::getString()
{
    QString s = QString("(") + BanRuleNames[static_cast<int>(id)] + ":";
    for (int i=0; i<prerequisites.count(); ++i) {
        s.append(prerequisites.at(i)->getString());
        if (i < prerequisites.count()-1) s.append(", ");
    }
    s.append(") -> (" + goal->getString() + ")");
    return s;
}

bool BANLogic::BanPostulates::operator ==(LPT::Postulate &p2)
{
    bool equals = true;
    BanPostulates *p = dynamic_cast<BanPostulates *>(&p2);
    if (p != nullptr) {
        // postulates are equivalent if they have same id and goals are equivalent,
        // have same number of prerequisites and each prerequisite is equivalet
        equals = id == p->id
                && *dynamic_cast<BanSComponent *>(goal) == *dynamic_cast<BanSComponent *>(p->goal)
                && prerequisites.count() == p->prerequisites.count();
        for (int i=0; equals && i<prerequisites.count(); ++i) {
            equals = equals && *dynamic_cast<BanSComponent *>(prerequisites.at(i)) == *dynamic_cast<BanSComponent *>(p->prerequisites.at(i));
        }
    } else {
        equals = false;
    }
    return equals;
}
}
