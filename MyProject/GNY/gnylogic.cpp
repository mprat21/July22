#include <iostream>

#include "gnylogic.h"
#include "gnyatom.h"
#include "gnydata.h"
#include "gnystatement.h"
#include "gnypostulate.h"
#include "../LPT/Postulate.h"
#include "../LPT/LPTExceptions.h"

namespace GNY {

GNYLogic::GNYLogic()
{
    // create Atoms used in postulates
    Atom *aP = new Atom(GNYAtomType::Principal);
    Atom *aQ = new Atom(GNYAtomType::Principal);
    Atom *aS = new Atom(GNYAtomType::Principal);

    Atom *aKPriv = new Atom(GNYAtomType::PrivKey);
    Atom *aKPub = new Atom(GNYAtomType::PubKey);
    aKPriv->setInverseKey(aKPub);
    aKPub->setInverseKey(aKPriv);
    Atom *aF = new Atom(GNYAtomType::Function);
    Atom *aH = new Atom(GNYAtomType::Hash);

    // create Data used in postulates
    Data *P = new Data(GNYDataType::Atom, aP);
    Data *Q = new Data(GNYDataType::Atom, aQ);
    Data *S = new Data(GNYDataType::Atom, aS);
    Data *K = new Data(GNYDataType::AnyData);
    Data *KPub = new Data(GNYDataType::Atom, aKPub);
    Data *KPriv = new Data(GNYDataType::Atom, aKPriv);
    Data *F = new Data(GNYDataType::Atom, aF);
    Data *H = new Data(GNYDataType::Atom, aH);
    Data *X = new Data(GNYDataType::AnyData);
    Data *Y = new Data(GNYDataType::AnyData);
    Data *Secret = new Data(GNYDataType::AnyData);

    // create Statements used in postulates
    Statement *c = new Statement();
    Statement *allStmt = new Statement(GNYStatementType::AllStatements, nullptr, GNYOperatorType::nop, nullptr);

    LPT::StatementPtrList preList;
    Statement *goal;
    Statement *pre;
    Data *d1, *d2, *d3, *d4;

    // create postulates - used { } to visually separate individual postulates

    // being told rules
    {
        // create T1: P told *X -> P told X
        preList.clear();
        d1 = new Data(GNYDataType::NotHereData, X);
        pre = new Statement(aP, GNYOperatorType::told, d1);
        preList.prepend(pre);
        goal = new Statement(aP, GNYOperatorType::told, X);
        postulates.append(new Postulate(GNYRuleID::T1, goal, preList));
    }

    {
        // create T2a: P told (X,Y) -> P told X
        preList.clear();
        d1 = new Data(GNYDataType::DataData, X, Y);
        pre = new Statement(aP, GNYOperatorType::told, d1);
        preList.append(pre);
        goal = new Statement(aP, GNYOperatorType::told, X);
        postulates.append(new Postulate(GNYRuleID::T2a, goal, preList));
    }

    {
        // create T2b: P told (X,Y) -> P told Y
        preList.clear();
        d1 = new Data(GNYDataType::DataData, X, Y);
        pre = new Statement(aP, GNYOperatorType::told, d1);
        preList.append(pre);
        goal = new Statement(aP, GNYOperatorType::told, Y);
        postulates.append(new Postulate(GNYRuleID::T2a, goal, preList));
    }

    {
        // create T3: P told {X}K, P possess K -> P told X
        preList.clear();
        d1 = new Data(GNYDataType::DataK, X, K);
        pre = new Statement(aP, GNYOperatorType::told, d1);
        preList.append(pre);
        pre = new Statement(aP, GNYOperatorType::possess, K);
        preList.append(pre);
        goal = new Statement(aP, GNYOperatorType::told, X);
        postulates.append(new Postulate(GNYRuleID::T3, goal, preList));
    }
    {
        // create T4: P told {X}K+, P possess K- -> P told X
        // requires that K+ and K- are linked (inverseKey property of Atom)
        preList.clear();
        d1 = new Data(GNYDataType::DataPubK, X, KPub);
        pre = new Statement(aP, GNYOperatorType::told, d1);
        preList.append(pre);
        pre = new Statement(aP, GNYOperatorType::possess, KPriv);
        preList.append(pre);
        goal = new Statement(aP, GNYOperatorType::told, X);
        postulates.append(new Postulate(GNYRuleID::T4, goal, preList));
    }

    {
        // create T5a: P told F(X,Y), P possess Y -> P told X
        preList.clear();
        d1 = new Data(GNYDataType::DataData, X, Y);
        d2 = new Data(GNYDataType::FData, F, d1);
        pre = new Statement(aP, GNYOperatorType::told, d2);
        preList.append(pre);
        pre = new Statement(aP, GNYOperatorType::possess, Y);
        preList.append(pre);
        goal = new Statement(aP, GNYOperatorType::told, X);
        postulates.append(new Postulate(GNYRuleID::T5a, goal, preList));
    }

    {
        // create T5b: P told F(X,Y), P possess X -> P told Y
        preList.clear();
        d1 = new Data(GNYDataType::DataData, X, Y);
        d2 = new Data(GNYDataType::FData, F, d1);
        pre = new Statement(aP, GNYOperatorType::told, d2);
        preList.append(pre);
        pre = new Statement(aP, GNYOperatorType::possess, X);
        preList.append(pre);
        goal = new Statement(aP, GNYOperatorType::told, Y);
        postulates.append(new Postulate(GNYRuleID::T5a, goal, preList));
    }

    {
        // create T4: P told {X}K-, P possess K+ -> P told X
        // requires that K+ and K- are linked (inverseKey property of Atom)
        preList.clear();
        d1 = new Data(GNYDataType::DataPubK, X, KPriv);
        pre = new Statement(aP, GNYOperatorType::told, d1);
        preList.append(pre);
        pre = new Statement(aP, GNYOperatorType::possess, KPub);
        preList.append(pre);
        goal = new Statement(aP, GNYOperatorType::told, X);
        postulates.append(new Postulate(GNYRuleID::T4, goal, preList));
    }


}

//////////////////////////////////////////////////////////////////////////
/// \brief GNYLogic::getMatchingPostulates returns a list of postulates
/// where the goal of the postulate unifies with stmt. Note: returned
/// postulates may not be fully instantiated.
///
/// \param stmt statement to match against postulate goals
/// \param pl out parameter, will carry all matching postulates
//////////////////////////////////////////////////////////////////////////
bool GNYLogic::getMatchingPostulates(LPT::Statement *stmt, LPT::PostulatePtrList &pl)
{
    // make sure that stmt is GNY::Statement
    Statement *s = dynamic_cast<Statement *>(stmt);
    bool foundPostulates = false;
    if (s != nullptr) {
        Postulate *p;
        Statement *goal = nullptr;
        // just be safe, cear pl
        pl.clear();

        // for each postulate in turn, create a copy and attempt to unify statement with postulate goal
        //    on success, add unified postulated to pl
        for (int i=0; i<postulates.count(); ++i) {
            p = dynamic_cast<Postulate *>(postulates.at(i)->getCopy());
            std::cout << postulates.at(i)->getString().toStdString() << " evaluate Postulate " << p->getString().toStdString() << std::endl;
            goal = dynamic_cast<Statement *>(p->getGoal());
            if (s->unify(goal)) {
                foundPostulates = true;
                pl.append(p);
                std::cout << "added Postulate " << p->getString().toStdString() << " to matching postulates" << std::endl;
            }
        }
    }
    return foundPostulates;
}

//////////////////////////////////////////////////////////////////////////
/// \brief GNYLogic::getAllInstantiatedPostulates returns a list of fully
/// instantiated postulates of p. Instantiations are based on already known
/// statements in LPT (requires back link to LPT).
/// \param p potentially partially instantiated postulate
/// \param ipl all sensible instantiations for p
//////////////////////////////////////////////////////////////////////////
bool GNYLogic::getAllInstantiatedPostulates(LPT::Postulate *p, LPT::PostulatePtrList &ipl)
{
    Postulate *instPostulate = dynamic_cast<Postulate *>(p->getCopy());
    Postulate *copy;
    GNYComponent *X;
    GNYComponent *Y;
    const Statement *s;
    ComponentPtrList instantiations;

    switch (instPostulate->getRuleID()) {
    case GNYRuleID::T1:
        // P told *X -> P told X
        // is fully instantiated - but do not apply T1 if goal is not here data
        X = instPostulate->getGNYGoal()->sPart;
        if (dynamic_cast<Data *>(X)->dType != GNYDataType::NotHereData && !ipl.contains(instPostulate)) {
            ipl.append(instPostulate);
        }
        break;
    case GNYRuleID::T2a:
        // P told X,Y -> P told X
        X = instPostulate->getGNYGoal()->sPart;
        // for each s in sigma
        for (int i=0; i<lpt->getKnownTrueStatementCount(); ++i) {
            s = dynamic_cast<const Statement *>(lpt->getKnownTrueStatement(i));
            // returns list of @partner@ components of X where parent is of type dataType
            // each element in dataList must be used to instantiate postulate
            s->findData(X, GNYDataType::DataData, instantiations, false, false);
        }

        // for each instantiation add instantiate postulate to ipl
        for (int i=0; i<instantiations.count(); ++i) {
            copy = new Postulate(*instPostulate);
            // instantiate Y component in prerequisite with found instantiations
            Y = dynamic_cast<Data *>(dynamic_cast<Statement *>(copy->getPrerequisites().at(0))->sPart)->dPart2;
            Y->instantiate(instantiations.at(i));
            if (!ipl.contains(copy)) {
                ipl.append(copy);
            }
        }
        break;
    case GNYRuleID::T2b:
    case GNYRuleID::T3:
    case GNYRuleID::T4:
    case GNYRuleID::T5a:
    case GNYRuleID::T5b:
    case GNYRuleID::T6:
        ;
        break;
    default:
        throw UnrecognisedPostulateException("Unrecognised Postulate Exception in GNY::Logic::getAllInstantiatedPostulates");
    }
}

} // end namespace GNY
