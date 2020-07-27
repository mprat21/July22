#include <iostream>
#include "gnypostulate.h"
#include "gnydefinitions.h"
#include "gnycomponent.h"
#include "gnystatement.h"

#include "../LPT/Postulate.h"


namespace GNY {

// define static members
PostulatePtrList GNYPostulate::allDynamicPostulates;


//////////////////////////////////////////////////////////////////////////
/// generic constructor - creates a GNY Postulate with given id, goal and
/// prerequisites
///
/// param:
/// pid - id for new postulate
/// pGoal - goal for new postulate
/// prereq - list of prerequisites for new postulate
//////////////////////////////////////////////////////////////////////////
GNYPostulate::GNYPostulate(GNYRuleID pid, LPT::Statement *pGoal, LPT::StatementPtrList &prereq)
    :id(pid)
{
    goal = pGoal;
    prerequisites.append(prereq);
    postulateName = GNYRuleNames[static_cast<int>(id)];
    rule = QString("(");
    for (int i=0; i<prerequisites.count(); ++i) {
        rule.append(prerequisites.at(i)->getString());
        if (i < prerequisites.count()-1) rule.append(", ");
    }
    rule.append(") -> (" + goal->getString() + ")");
    updateAtomicComponents();
}

//////////////////////////////////////////////////////////////////////////
/// Copy constructor
//////////////////////////////////////////////////////////////////////////
GNYPostulate::GNYPostulate(GNYPostulate &orig)
    :rule(orig.rule), id(orig.id)
{
    GNYComponent *c;
    ComponentPtrList copyComponents;
    // make sure orig.allAtomicComponents is up to date
    orig.updateAtomicComponents();
    // create copies of all components
    for (int i=0; i<orig.allAtomicComponents.count(); ++i) {
        c = orig.allAtomicComponents.at(i);
        if (c->getComponentType() == GNYComponentType::Atom) {
            copyComponents.append(new GNYAtom(*dynamic_cast<GNYAtom *>(c)));
        } else if (c->getComponentType() == GNYComponentType::Data) {
            copyComponents.append(new GNYData(*dynamic_cast<GNYData *>(c)));
        }
    }
    postulateName = orig.postulateName;
    goal = dynamic_cast<GNYStatement *>(orig.goal)->getCopy(copyComponents);
    for (int i=0; i<orig.prerequisites.count(); ++i) {
        prerequisites.append((dynamic_cast<GNYStatement *>(orig.prerequisites.at(i)))->getCopy(copyComponents));
    }
    updateAtomicComponents();
}

//////////////////////////////////////////////////////////////////////////
/// destructor
//////////////////////////////////////////////////////////////////////////
GNYPostulate::~GNYPostulate()
{
    delete goal;
    prerequisites.clear();
    allAtomicComponents.clear();
}

//////////////////////////////////////////////////////////////////////////
/// \brief Postulate::updateAtomicComponents
/// ensures that all atomic components appear once in list allAtomicComponents
void GNYPostulate::updateAtomicComponents()
{
    GNYComponent *c;
    GNYAtom *a;
    GNYData *d;
    GNYStatement *s;

    // clear old components
    allAtomicComponents.clear();

    LPT::LPTStack<GNYComponent *> stack;
    // push goal and all prerequisites onto stack
    stack.push(getGNYGoal());
    for (int i=0; i<prerequisites.count(); ++i) {
        stack.push(new GNYStatement(*dynamic_cast<GNYStatement *>(prerequisites.at(i))));
    }
    while (!stack.isEmpty()) {
        c = stack.pop();
        switch (c->getComponentType()){
        case GNYComponentType::Atom:
            a = dynamic_cast<GNYAtom *>(c);
            if (!allAtomicComponents.containsPtrToEquivalentObject(a)) {
                allAtomicComponents.append(a);
            }
            break;
        case GNYComponentType::Data:
            // if type is AnyData, append itsefl to list
            // otherwise push all non-null parts onto stack
            d = dynamic_cast<GNYData *>(c);
            if (d->dType == GNYDataType::AnyData) {
                if (!allAtomicComponents.containsPtrToEquivalentObject(d)) {
                    allAtomicComponents.append(d);
                }
            } else {
                if (d->dPart1 != nullptr) stack.push(d->dPart1);
                if (d->dPart2 != nullptr) stack.push(d->dPart2);
                if (d->dPart3 != nullptr) stack.push(d->dPart3);
            }
            break;
        case GNYComponentType::Statement:
            s = dynamic_cast<GNYStatement *>(c);
            // add principal to list
            if (!allAtomicComponents.containsPtrToEquivalentObject(s->sPrincipal)) {
                allAtomicComponents.append(s->sPrincipal);
            }
            // push part onto stack
            if (s->sPart != nullptr) stack.push(s->sPart);
            break;
        default:
            throw UnrecognisedComponentException("Unrecognised component in GNY::Postulate::updateAtomicComponents");
        }
    }


}

QString GNYPostulate::getString()
{
    QString s = QString("(") + GNYRuleNames[static_cast<int>(id)] + ":";
    for (int i=0; i<prerequisites.count(); ++i) {
        s.append(prerequisites.at(i)->getString());
        if (i < prerequisites.count()-1) s.append(", ");
    }
    s.append(") -> (" + goal->getString() + ")");
    return s;
}

//////////////////////////////////////////////////////////////////////////
/// overloaded new operator ensures that objects allocated with new are
/// added to allDynamicPostulate
//////////////////////////////////////////////////////////////////////////
void *GNYPostulate::operator new(size_t size)
{
    void *p = malloc(size);
    // keep track of all dynamically created objects by adding p to allDynamicAtoms
    allDynamicPostulates.append(static_cast<GNYPostulate *>(p));
    return p;
}

//////////////////////////////////////////////////////////////////////////
/// overloaded delete operator ensures that objects de-allocated with
/// delete are removed from allDynamicAtoms
//////////////////////////////////////////////////////////////////////////
void GNYPostulate::operator delete(void *ptr)
{
    allDynamicPostulates.removeOne(static_cast<GNYPostulate *>(ptr));
    free(ptr);
}

//////////////////////////////////////////////////////////////////////////
/// operator==
/// compares two postulates and returns true if they are equivalent,
/// false otherwise
///
/// param:
/// p2 - postulate to be copied
//////////////////////////////////////////////////////////////////////////
bool GNYPostulate::operator==(LPT::Postulate &p2)
{
    bool equals = true;
    GNYPostulate *p = dynamic_cast<GNYPostulate *>(&p2);
    if (p != nullptr) {
        // postulates are equivalent if they have same id and goals are equivalent,
        // have same number of prerequisites and each prerequisite is equivalet
        equals = id == p->id
                && *dynamic_cast<GNYComponent *>(goal) == *dynamic_cast<GNYComponent *>(p->goal)
                && prerequisites.count() == p->prerequisites.count();
        for (int i=0; equals && i<prerequisites.count(); ++i) {
            equals = equals && *dynamic_cast<GNYComponent *>(prerequisites.at(i)) == *dynamic_cast<GNYComponent *>(p->prerequisites.at(i));
        }
    } else {
        equals = false;
    }
    return equals;
}

//////////////////////////////////////////////////////////////////////////
/// returns a copy of postulate
/// Interface method to copy constuctor for use by LPT
//////////////////////////////////////////////////////////////////////////
LPT::Postulate *GNYPostulate::getCopy() {
    return new GNYPostulate(*this);
}



}
