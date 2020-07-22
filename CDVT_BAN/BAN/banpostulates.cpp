#include "bandatac.h"
#include "banpostulates.h"

using namespace BANLogic;
namespace BANLogic {

QString BanPostulates::getRule() const
{
    return rule;
}

//QList<BanStatementList *> BanPostulates::getBanPrerequisites(BanRuleId r) const
//{
//    QList<BanStatementList*> preReq;
//    preReq.clear();
//    foreach(BanPostulates *pos, allPostulate)
//    {
//        if(pos->getRuleID()==r)
//            preReq.append(pos->temp);
//    }
//    return preReq;
//}

QList<BanSComponent *> BanPostulates::getPComponentList() const
{
    return pComponentList;
}

void BanPostulates::print()
{
    QList<BanStatementList *> preList;
    QTextStream(stdout)<< "Postulate ID: " << rule;
    cout << endl << "Pre:\n";
    //preList=getBanPrerequisites(getRuleID());
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
   // preList=getBanPrerequisites(getRuleID());
    foreach(BanStatementList *pre,preList)
    {
        pre->printRPN();
        cout << endl;
    }
    QTextStream(stdout) << "\nGoal:\n";goals->printRPN();
    cout << endl;

}

void BanPostulates::updateAtomicComponents()
{
    BanSComponent *c;
    BanDComponent *cD;
    BanDAtom *a;
    BanDataC *d;
    BanDataList *mydList;
    BanStatementList *s;
    BanDOperator *o;
    BanSOperator *oS;
    // clear old components
    allAtomicComponents.clear();
    LPT::LPTStack<BanSComponent *> stack;
    // push goal and all prerequisites onto stack
    stack.push(getBanGoal());
    for (int i=0; i<prerequisites.count(); ++i) {
        stack.push(new BanStatementList(*dynamic_cast<BanStatementList *>(prerequisites.at(i))));
    }
    while (!stack.isEmpty())
    {
        c = stack.pop();
        switch (c->getStype()){
        case BanSComponentType::bData:
        {
            mydList = dynamic_cast<BanDataList *>(c);
            QList<BanDComponent*> listofcomps=mydList->getDataList();
            for(int i=0; i<listofcomps.count(); i++)
            {
                switch(listofcomps.value(i)->getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    a = dynamic_cast<BanDAtom *>(listofcomps.value(i));
                    if (!allAtomicComponents.containsPtrToEquivalentObject(a)) {
                        allAtomicComponents.append(a);
                    }
                    break;
                }
                case BanDComponentType::bOperator:
                {
                    o = dynamic_cast<BanDOperator *>(listofcomps.value(i));
                    break;
                }
                case BanDComponentType::bAnyData:
                {
                    d = dynamic_cast<BanDataC *>(listofcomps.value(i));
                   if(d->getInstantiate()==false)
                       break;
                    else if (!allAtomicComponents.containsPtrToEquivalentObject(d)) {
                        allAtomicComponents.append(d);
                    }
                    break;
                }
                }
            }
        }
        case BanSComponentType::bSOperator:
        {
            oS = dynamic_cast<BanSOperator*>(c);
            //  if (oS != nullptr) stack.push(oS);
            break;
        }
        case BanSComponentType::bStatement:
        {
            s = dynamic_cast<BanStatementList *>(c);
           QList<BanSComponent*> li= s->getStList();
           for(int i=0; i<li.count(); i++)
           {
               if (li.value(i) != nullptr && (li.value(i)->getStype()!=BanSComponentType::bSOperator))
                   stack.push(li.value(i));
           }

            break;
        }
        default:
            throw UnrecognisedComponentException("Unrecognised component in BAN::Postulate::updateAtomicComponents");
        }

    }
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
    updateAtomicComponents();
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
    updateAtomicComponents();
    //cout<<rule.toStdString()<<endl;
}

BanPostulates::BanPostulates(BanPostulates &orig):rule(orig.rule),id(orig.id)
{
    BanDComponent *c;
    LPT::LPTPtrList<BanDComponent> copyComponents;
    orig.updateAtomicComponents();
    for (int i=0; i<orig.allAtomicComponents.count(); ++i) {
        c = orig.allAtomicComponents.at(i);
        if (c->getDtype() == BanDComponentType::bAtom) {
            copyComponents.append(new BanDAtom(*dynamic_cast<BanDAtom *>(c)));
        } else if (c->getDtype() == BanDComponentType::bAnyData) {
            copyComponents.append(new BanDataC(*dynamic_cast<BanDataC *>(c)));
        }
    }
    postulateName = orig.postulateName;
    goal = dynamic_cast<BanStatementList *>(orig.goal)->getCopy(copyComponents);
    for (int i=0; i<orig.prerequisites.count(); ++i) {
        prerequisites.append((dynamic_cast<BanStatementList *>(orig.prerequisites.at(i)))->getCopy(copyComponents));
    }
    updateAtomicComponents();


//    id=orig.getRuleID();
//    rule=orig.rule;
//    postulateName = orig.postulateName;
//    BanPostulates *newPt=new BanPostulates();
//    newPt->goals= dynamic_cast<BanStatementList *>(orig.goal);
//    goal=newPt->goals;
//    for (int i=0; i<orig.prerequisites.count(); ++i) {
//        newPt->prereq.append(dynamic_cast<BanStatementList *>(orig.prerequisites.at(i)));
//        prerequisites.append(newPt->prereq.at(i));
//    }
//    updateAtomicComponents();



}

LPT::Postulate *BANLogic::BanPostulates::getCopy()
{
    return new BanPostulates(*this);
}

QString BANLogic::BanPostulates::getString()
{
    QString s = QString("");
    s.append("(" + BanRuleNames[(int)(id)] + ":");
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
