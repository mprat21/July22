#include <iostream>

#include "gnylogic.h"
#include "atom.h"
#include "gnydata.h"
#include "gnystatement.h"
#include "gnypostulate.h"
#include "../LPT/Postulate.h"
#include "../LPT/LPTExceptions.h"

namespace GNY {

GNYLogic::GNYLogic()
{
    // create Atoms used in postulates
    GNYAtom *aP = new GNYAtom(GNYAtomType::Principal);
    GNYAtom *aQ = new GNYAtom(GNYAtomType::Principal);
    GNYAtom *aS = new GNYAtom(GNYAtomType::Principal);

    GNYAtom *aKPriv = new GNYAtom(GNYAtomType::PrivKey);
    GNYAtom *aKPub = new GNYAtom(GNYAtomType::PubKey);
    aKPriv->setInverseKey(aKPub);
    aKPub->setInverseKey(aKPriv);
    GNYAtom *aF = new GNYAtom(GNYAtomType::Function);
    GNYAtom *aH = new GNYAtom(GNYAtomType::Hash);

    // create Data used in postulates
    GNYData *P = new GNYData(GNYDataType::Atom, aP);
    GNYData *Q = new GNYData(GNYDataType::Atom, aQ);
    GNYData *S = new GNYData(GNYDataType::Atom, aS);
    GNYData *K = new GNYData(GNYDataType::AnyData);
    GNYData *KPub = new GNYData(GNYDataType::Atom, aKPub);
    GNYData *KPriv = new GNYData(GNYDataType::Atom, aKPriv);
    GNYData *F = new GNYData(GNYDataType::Atom, aF);
    GNYData *H = new GNYData(GNYDataType::Atom, aH);
    GNYData *X = new GNYData(GNYDataType::AnyData);
    GNYData *Y = new GNYData(GNYDataType::AnyData);
    GNYData *Secret = new GNYData(GNYDataType::AnyData);

    // create Statements used in postulates
    GNYStatement *c = new GNYStatement();
    GNYStatement *allStmt = new GNYStatement(GNYStatementType::AllStatements, nullptr, GNYOperatorType::nop, nullptr);

    LPT::StatementPtrList preList;
    GNYStatement *goal;
    GNYStatement *pre;
    GNYData *d1, *d2, *d3, *d4;

    // create postulates - used { } to visually separate individual postulates

    // being told rules

    {
        // create T1: P told *X -> P told X
        preList.clear();
        d1 = new GNYData(GNYDataType::NotHereData, X);
        pre = new GNYStatement(aP, GNYOperatorType::told, d1);
        preList.prepend(pre);
        goal = new GNYStatement(aP, GNYOperatorType::told, X);
        postulates.append(new GNYPostulate(GNYRuleID::T1, goal, preList));
    }

    {
        // create T2a: P told (X,Y) -> P told X
        preList.clear();
        d1 = new GNYData(GNYDataType::DataData, X, Y);
        pre = new GNYStatement(aP, GNYOperatorType::told, d1);
        preList.append(pre);
        goal = new GNYStatement(aP, GNYOperatorType::told, X);
        postulates.append(new GNYPostulate(GNYRuleID::T2a, goal, preList));
    }

    {
        // create T2b: P told (X,Y) -> P told Y
        preList.clear();
        d1 = new GNYData(GNYDataType::DataData, X, Y);
        pre = new GNYStatement(aP, GNYOperatorType::told, d1);
        preList.append(pre);
        goal = new GNYStatement(aP, GNYOperatorType::told, Y);
        postulates.append(new GNYPostulate(GNYRuleID::T2b, goal, preList));
    }

    {
        // create T3: P told {X}K, P possess K -> P told X
        preList.clear();
        d1 = new GNYData(GNYDataType::DataK, X, K);
        pre = new GNYStatement(aP, GNYOperatorType::told, d1);
        preList.append(pre);
        pre = new GNYStatement(aP, GNYOperatorType::possess, K);
        preList.append(pre);
        goal = new GNYStatement(aP, GNYOperatorType::told, X);
        postulates.append(new GNYPostulate(GNYRuleID::T3, goal, preList));
    }
    {
        // create T4: P told {X}K+, P possess K- -> P told X
        // requires that K+ and K- are linked (inverseKey property of Atom)
        preList.clear();
        d1 = new GNYData(GNYDataType::DataPubK, X, KPub);
        pre = new GNYStatement(aP, GNYOperatorType::told, d1);
        preList.append(pre);
        pre = new GNYStatement(aP, GNYOperatorType::possess, KPriv);
        preList.append(pre);
        goal = new GNYStatement(aP, GNYOperatorType::told, X);
        postulates.append(new GNYPostulate(GNYRuleID::T4, goal, preList));
    }

    {
        // create T5a: P told F(X,Y), P possess Y -> P told X
        preList.clear();
        d1 = new GNYData(GNYDataType::DataData, X, Y);
        d2 = new GNYData(GNYDataType::FData, F, d1);
        pre = new GNYStatement(aP, GNYOperatorType::told, d2);
        preList.append(pre);
        pre = new GNYStatement(aP, GNYOperatorType::possess, Y);
        preList.append(pre);
        goal = new GNYStatement(aP, GNYOperatorType::told, X);
        postulates.append(new GNYPostulate(GNYRuleID::T5a, goal, preList));
    }

    {
        // create T5b: P told F(X,Y), P possess X -> P told Y
        preList.clear();
        d1 = new GNYData(GNYDataType::DataData, X, Y);
        d2 = new GNYData(GNYDataType::FData, F, d1);
        pre = new GNYStatement(aP, GNYOperatorType::told, d2);
        preList.append(pre);
        pre = new GNYStatement(aP, GNYOperatorType::possess, X);
        preList.append(pre);
        goal = new GNYStatement(aP, GNYOperatorType::told, Y);
        postulates.append(new GNYPostulate(GNYRuleID::T5b, goal, preList));
    }

    {
        // create T6: P told {X}K-, P possess K+ -> P told X
        // requires that K+ and K- are linked (inverseKey property of Atom)
        preList.clear();
        d1 = new GNYData(GNYDataType::DataPubK, X, KPriv);
        pre = new GNYStatement(aP, GNYOperatorType::told, d1);
        preList.append(pre);
        pre = new GNYStatement(aP, GNYOperatorType::possess, KPub);
        preList.append(pre);
        goal = new GNYStatement(aP, GNYOperatorType::told, X);
        postulates.append(new GNYPostulate(GNYRuleID::T6, goal, preList));
    }

    // possesion rules
    {
        preList.clear();
        pre = new GNYStatement(aP, GNYOperatorType::told, X);
        preList.append(pre);
        goal = new GNYStatement(aP, GNYOperatorType::possess, X);
        postulates.append(new GNYPostulate(GNYRuleID::P1, goal, preList));
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
    GNYStatement *s = dynamic_cast<GNYStatement *>(stmt);
    bool foundPostulates = false;
    if (s != nullptr) {
        GNYPostulate *p;
        GNYStatement *goal = nullptr;
        // just be safe, cear pl
        pl.clear();

        // for each postulate in turn, create a copy and attempt to unify statement with postulate goal
        //    on success, add unified postulated to pl
        for (int i=0; i<postulates.count(); ++i) {
            p = dynamic_cast<GNYPostulate *>(postulates.at(i)->getCopy());
            goal = dynamic_cast<GNYStatement *>(p->getGoal());
            if (s->unify(goal)) {
                foundPostulates = true;
                pl.append(p);
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
    GNYPostulate *instPostulate = dynamic_cast<GNYPostulate *>(p->getCopy());
    GNYPostulate *copy;
    GNYComponent *X;
    GNYComponent *Y;
    const GNYStatement *s;
    ComponentPtrList instantiations;

    switch (instPostulate->getRuleID()) {
    case GNYRuleID::P1:
        if (!ipl.contains(instPostulate)) {
            ipl.append(instPostulate);
        }
        break;
    case GNYRuleID::T1: // P told *X -> P told X
        // is fully instantiated - but do not apply T1 if goal is not here data
        X = instPostulate->getGNYGoal()->sPart;
        if ( (dynamic_cast<GNYData *>(X)->dType != GNYDataType::NotHereData) && !ipl.contains(instPostulate)) {
            ipl.append(instPostulate);
        }
        break;
    case GNYRuleID::T2a: // P told X,Y -> P told X
        X = instPostulate->getGNYGoal()->sPart;
        // for each s in sigma
        for (int i=0; i<lpt->getKnownTrueStatementCount(); ++i) {
            s = dynamic_cast<const GNYStatement *>(lpt->getKnownTrueStatement(i));
            // returns list of @partner@ components of X where parent is of type dataType
            // each element in dataList must be used to instantiate postulate
            s->findData(X, GNYDataType::DataData, instantiations, false, false);
        }
        // for each instantiation add instantiate postulate to ipl
        for (int i=0; i<instantiations.count(); ++i) {
            copy = new GNYPostulate(*instPostulate);
            // instantiate Y component in prerequisite with found instantiations
            Y = dynamic_cast<GNYData *>(dynamic_cast<GNYStatement *>(copy->getPrerequisites().at(0))->sPart)->dPart2;
            Y->instantiate(instantiations.at(i));
            if (!ipl.contains(copy)) {
                ipl.append(copy);
            }
        }
        break;
    case GNYRuleID::T2b: // create T2b: P told (X,Y) -> P told Y
        Y = instPostulate->getGNYGoal()->sPart;
        // for each s in sigma
        for (int i=0; i<lpt->getKnownTrueStatementCount(); ++i) {
            s = dynamic_cast<const GNYStatement *>(lpt->getKnownTrueStatement(i));
            // returns list of @partner@ components of X where parent is of type dataType
            // each element in dataList must be used to instantiate postulate
            s->findData(Y, GNYDataType::DataData, instantiations, true, false);
        }
        // for each instantiation add instantiate postulate to ipl
        for (int i=0; i<instantiations.count(); ++i) {
            copy = new GNYPostulate(*instPostulate);
            // instantiate Y component in prerequisite with found instantiations
            X = dynamic_cast<GNYData *>(dynamic_cast<GNYStatement *>(copy->getPrerequisites().at(0))->sPart)->dPart1;
            X->instantiate(instantiations.at(i));
            if (!ipl.contains(copy)) {
                ipl.append(copy);
            }
        }
        break;
    case GNYRuleID::T3: // P told {X}K, P possess K -> P told X
        X = instPostulate->getGNYGoal()->sPart;
        // for each s in sigma
        for (int i=0; i<lpt->getKnownTrueStatementCount(); ++i) {
            s = dynamic_cast<const GNYStatement *>(lpt->getKnownTrueStatement(i));
            // returns list of @partner@ components of X where parent is of type dataType
            // each element in dataList must be used to instantiate postulate
            s->findData(X, GNYDataType::DataK, instantiations, false, false);
        }
        // for each instantiation add instantiate postulate to ipl
        for (int i=0; i<instantiations.count(); ++i) {
            copy = new GNYPostulate(*instPostulate);
            // instantiate Y component in prerequisite with found instantiations
            Y = dynamic_cast<GNYData *>(dynamic_cast<GNYStatement *>(copy->getPrerequisites().at(0))->sPart)->dPart2;
            Y->instantiate(instantiations.at(i));
            if (!ipl.contains(copy)) {
                ipl.append(copy);
            }
        }
        break;
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
