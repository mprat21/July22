#include "gnyatom.h"
#include "gnydata.h"

namespace GNY {

// static member definition/initialisation
int Atom::atomCount = 0;
ComponentPtrList Atom::allDynamicAtoms;

// creates a GNY Atom with given properties
Atom::Atom(GNYAtomType at, QString identifier)
    :GNYComponent(GNYComponentType::Atom), aType(at), inverseKey(nullptr), atomID("")
{
    // empty identifier signifies "free" atom (not instantiated)
    if (identifier==nullptr || identifier.isEmpty()) {
        switch (at) {
        case GNYAtomType::Principal:
            atomID += QString("_P_") + QString::number(atomCount++);
            break;
        case GNYAtomType::SymKey:
            atomID += QString("_K_") + QString::number(atomCount++);
            break;
        case GNYAtomType::PubKey:
            atomID += QString("_K+_") + QString::number(atomCount++);
            break;
        case GNYAtomType::PrivKey:
            atomID += QString("_K-_") + QString::number(atomCount++);
            break;
        case GNYAtomType::Nonce:
            atomID += QString("_N_") + QString::number(atomCount++);
            break;
        case GNYAtomType::Timestamp:
            atomID += QString("_TS_") + QString::number(atomCount++);
            break;
        case GNYAtomType::Function:
            atomID += QString("_F_") + QString::number(atomCount++);
            break;
        case GNYAtomType::Hash:
            atomID += QString("_H_") + QString::number(atomCount++);
            break;
        case GNYAtomType::BinaryData:
            atomID += QString("_data_") + QString::number(atomCount++);
            break;
        default:
            throw UnrecognisedAtomException("Unrecognised Atom-type in GNY Atom Constructor");
        }
        instantiated = false;
    } else {
        atomID = identifier;
        instantiated = true;
        ++atomCount;
    }
}

//////////////////////////////////////////////////////////////////////////
/// As Atom has no dynamic members, no destructor is required.
/// //////////////////////////////////////////////////////////////////////
Atom::~Atom()
{

}


//////////////////////////////////////////////////////////////////////////
/// overloaded new operator ensures that objects allocated with new are
/// added to allDynamicAtoms
//////////////////////////////////////////////////////////////////////////
void *Atom::operator new(size_t size)
{
    void *p = malloc(size);
    // keep track of all dynamically created objects by adding p to allDynamicAtoms
    allDynamicAtoms.append(static_cast<Atom *>(p));
    return p;
}

//////////////////////////////////////////////////////////////////////////
/// overloaded delete operator ensures that objects de-allocated with
/// delete are removed from allDynamicAtoms
//////////////////////////////////////////////////////////////////////////
void Atom::operator delete(void *ptr)
{
    allDynamicAtoms.removeOne(static_cast<Atom *>(ptr));
    free(ptr);
}


/// returns true if current object is identical to other
bool Atom::operator==(const GNYComponent &other)
{
    bool equals = true;
    const Atom &a = dynamic_cast<const Atom &>(other);
    if (aType != a.aType) {
        equals = false;
    } else {
        equals = atomID == a.atomID;
    }
    return equals;
}

//////////////////////////////////////////////////////////////////////////
/// static method that deletes all remaining dynamic atom objects
//////////////////////////////////////////////////////////////////////////
void Atom::deleteAllDynamicAtoms()
{
    // delete will also remove object from list
    for (int i=allDynamicAtoms.count(); i>0; --i) {
        delete allDynamicAtoms.at(i-1);
    }
}

//////////////////////////////////////////////////////////////////////////
/// sets the inverse key for public/private key pairs
/// parameter: invKey - inverse key for current key
//////////////////////////////////////////////////////////////////////////
void Atom::setInverseKey(Atom *invKey)
{
    if (aType==GNYAtomType::PubKey || aType==GNYAtomType::PrivKey)
        inverseKey = invKey;
    else throw LPTException("Attempting to set inverse key on non public/private key");
}

//////////////////////////////////////////////////////////////////////////
/// instantiated current atom with given component (which must be an atom)
/////////////////////////////////////////////////////////////////////////
void Atom::instantiate(GNYComponent *c)
{
    if (instantiated) throw LPTException("Attempting to instantiate already instantiated atom");
    else {
        if (c->getComponentType() == GNYComponentType::Atom) {
            Atom *orig = dynamic_cast<Atom *>(c);
            atomID = orig->atomID;
            aType = orig->aType;
            instantiated = orig->instantiated;
        } else {
            throw IllegalComponentTypeException("Illegal component type in GNY Atom instantiate");
        }
    }
}

// attempts to unify current atom with c
// this method will only modify c to make both equivalent. current object will not be modified.
// param: c - component that is to be unified with current atom
bool Atom::unify(GNYComponent *c)
{
    // assume both unify - falsify on countrary evidence
    bool unifies = true;
    Data *d;
    Atom *a;

    // evaluate that this is instantiated
    if (!instantiated) {
        throw LPTException("Atom::unify called on not instantiated atom");
    } else {

        if (c != nullptr) {
            switch (c->getComponentType()) {
            case GNYComponentType::Statement:
                // statement and atom never unify
                unifies = false;
                break;

            case GNYComponentType::Data:
                d = dynamic_cast<Data *>(c);
                // data and atom only unify if data is anydata
                if (unifies = (d->getDataType() == GNYDataType::AnyData)) {
                    // change AnyData object to type Atom with current object
                    d->dType = GNYDataType::Atom;
                    d->dPart1 = this;
                }
                break;

            case GNYComponentType::Atom:
                a = dynamic_cast<Atom *>(c);
                // only atoms of same type can unify
                if (aType != a->aType) {
                    unifies = false;
                } else if (instantiated && a->instantiated) {
                     // two instantiated atoms only unify if both atomID are equal
                     unifies = (atomID==a->atomID);
                } else if (!a->instantiated) {
                    // not instantiated atom always unifies with instantiated atom
                    a->atomID = atomID;
                    a->instantiated = true;
                    a->inverseKey = inverseKey;
                } else {
                    // should not appear
                    throw LPTException("final else in GNY::Atom::unify");
                }
                break;

            default:
                throw IllegalComponentTypeException("Illegal component type in GNY Atom unify");
            }
        }
    }
    return unifies;
}

/// match compares structure of current atom with C and returns true if they match.
/// match does not modify c in any way
/// note: match is a prerequisite for unify, but does not imply unify
bool Atom::match(GNYComponent *c)
{
    // assume both match - falsify on countrary evidence
    bool match = true;
    Data *d;
    Atom *a;

    // evaluate that this is instantiated
    if (!instantiated) {
        throw LPTException("Atom::match called on not instantiated atom");
    } else {
        if (c != nullptr) {
            switch (c->getComponentType()) {
            case GNYComponentType::Statement:
                // statement and atom never match
                match = false;
                break;

            case GNYComponentType::Data:
                d = dynamic_cast<Data *>(c);
                // data and atom only unify if data is anydata
                match = (d->getDataType() == GNYDataType::AnyData);
                break;

            case GNYComponentType::Atom:
                a = dynamic_cast<Atom *>(c);
                if (aType != a->aType) {
                    match = false;
                } else if (instantiated && a->instantiated) {
                    // instantiated atoms only match if they have same id
                    match = atomID==a->atomID;
                } else {
                    match = false;
                }
                break;
            default:
                throw IllegalComponentTypeException("Illegal component type in GNY Atom unify");
            }
        }
    }
    return match;
}

} // end namespace GNY
