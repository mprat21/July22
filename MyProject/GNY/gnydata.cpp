#include "gnydata.h"
#include "gnylogic.h"
#include "../LPT/Layeredprovingtree.h"


namespace GNY {

// static member definition/initialisation
int Data::dataCount = 0;
ComponentPtrList Data::allDynamicData;

/// default constructor - creates a data object of type anydata
Data::Data()
    :GNYComponent(GNYComponentType::Data), dType(GNYDataType::AnyData),
      dPart1(nullptr), dPart2(nullptr), dPart3(nullptr)
{
    dataID = QString("D") + QString::number(dataCount++);
}


/// general constructor - creates data with particular type and components
Data::Data(GNYDataType dt, GNYComponent *comp1, GNYComponent *comp2, GNYComponent *comp3)
    :GNYComponent(GNYComponentType::Data), dType(dt), dPart1(comp1), dPart2(comp2), dPart3(comp3)
{
    dataID = QString("D") + QString::number(dataCount++);
}

// copy consructor - creates a copy of orig
Data::Data(Data &orig)
    :GNYComponent(GNYComponentType::Data), dType(orig.dType), dPart1(nullptr), dPart2(nullptr), dPart3(nullptr)
{
    dataID = orig.dataID;
    instantiate(&orig);
}

Data::~Data()
{
    // simply delete all parts if not null
    if (dPart1 != nullptr) delete dPart1;
    if (dPart2 != nullptr) delete dPart2;
    if (dPart3 != nullptr) delete dPart3;
}

//////////////////////////////////////////////////////////////////////////
/// overloaded new operator ensures that objects allocated with new are
/// added to allDynamicData
//////////////////////////////////////////////////////////////////////////
void *Data::operator new(size_t size)
{
    void *p = malloc(size);
    // keep track of all dynamically created objects by adding p to allDynamicAtoms
    allDynamicData.append(static_cast<Data *>(p));
    return p;
}

//////////////////////////////////////////////////////////////////////////
/// overloaded delete operator ensures that objects de-allocated with
/// delete are removed from allDynamicAtoms
//////////////////////////////////////////////////////////////////////////
void Data::operator delete(void *ptr)
{
    allDynamicData.removeOne(static_cast<Data *>(ptr));
    free(ptr);
}

Data *Data::getCopy(ComponentPtrList &components)
{
    Data *copy = new Data();
    GNYComponent *c;
    Data *d;
    Atom *a;
    switch (dType) {
    case GNYDataType::Atom:
        // search for atom in components
        for (int i=0; i<components.count(); ++i) {
            c = components.at(i);
            if ( (c->getComponentType() == GNYComponentType::Atom) && (*c == *dPart1)) {
               // found component, set it in copy
               copy->dPart1 = a;
               break;
            }
        }
        break;
    case GNYDataType::AnyData:
        // search for anydata in components
        for (int i=0; i<components.count(); ++i) {
            c = components.at(i);
            if (c->getComponentType()==GNYComponentType::Data && *c == *this) {
                delete copy;
                copy = dynamic_cast<Data *>(c);
                break;
            }
        }
        break;
    case GNYDataType::DataExtendStmt:
        throw LPTException("GNY::data::getCopy DataExtendStmt not implemented yet");
        break;
    default:
        // create copies of all non-null parts
        if (dPart1 != nullptr) copy->dPart1 = dynamic_cast<Data *>(dPart1)->getCopy(components);
        if (dPart2 != nullptr) copy->dPart2 = dynamic_cast<Data *>(dPart2)->getCopy(components);
        if (dPart3 != nullptr) copy->dPart3 = dynamic_cast<Data *>(dPart3)->getCopy(components);
    }
    return copy;
}

/// returns true if current object is identical to other
bool Data::operator==(const GNYComponent &other)
{
    bool equals = true;
    const Data &d = dynamic_cast<const Data &>(other);
    if (dType != d.dType) {
        equals = false;
    } else if (dType == GNYDataType::AnyData) {
        equals = dataID == d.dataID;
    } else {
        if (dPart1 != nullptr) equals = equals && (*dPart1 == *d.dPart1);
        if (dPart2 != nullptr) equals = equals && (*dPart2 == *d.dPart2);
        if (dPart3 != nullptr) equals = equals && (*dPart3 == *d.dPart3);
    }
    return equals;
}


//////////////////////////////////////////////////////////////////////////
/// static method that deletes all remaining dynamic atom objects
//////////////////////////////////////////////////////////////////////////
void Data::deleteAllDynamicData()
{
    // delete will also remove object from list
    for (int i=allDynamicData.count(); i>0; --i) {
        delete allDynamicData.at(i-1);
    }
}


//////////////////////////////////////////////////////////////////////////
/// findData(c, dt, instantiations, findFirstComponent, extend)
/// searches for data types d of type dt where one component is equivalent
/// to c and adds the other component of d to instantiations.
/// if findFirstComponent is true, c holds second component of d (and
/// first is added to instantiations - otherwise c holds first component and
/// second component of d is added to instantiations.
/// if dt is ShareKey or ShareSecret, then if findFirstComponent is true c holds
/// value of key/secret and both principals are added to instantiations - otherwise
/// c holds a principal and key/secret is added to instantiations.
///
/// param:
/// c - known component
/// dt - gny data type of parent data
/// instantiations - list to hold all found partner components
/// findFirstComponent - indicates whether c holds first/second component and
///     second/first component is added to instantiations
/// //////////////////////////////////////////////////////////////////////////
void Data::findData(GNYComponent *c, GNYDataType dt, ComponentPtrList &instantiatons, bool findFirstComponent,
                  bool extend) const
{
    Data *d;
    Statement *s;
    if (dType == dt) {
        d = dynamic_cast<Data *>(c);
        switch (d->dType) {
        case GNYDataType::Atom:
            throw IllegalDataTypeException("Atom should not be used in GNY::Data::find()");
            break;
        case GNYDataType::RecogniseData:
            throw IllegalDataTypeException("RecognisedData should not be used in GNY::Data::find()");
            break;
        case GNYDataType::FreshData:
            throw IllegalDataTypeException("FreshData should not be used in GNY::Data::find()");
            break;
        case GNYDataType::NotHereData:
            throw IllegalDataTypeException("NoHereData should not be used in GNY::Data::find()");
            break;
        case GNYDataType::AnyData:
            throw IllegalDataTypeException("AnyData should not be used in GNY::Data::find()");
            break;

        case GNYDataType::DataData:
        case GNYDataType::DataK:
        case GNYDataType::DataPubK:
        case GNYDataType::DataPrivK:
        case GNYDataType::FData:
        case GNYDataType::HData:
        case GNYDataType::HasKey:
        case GNYDataType::DataExtendStmt:
            if (findFirstComponent) {
                // d is second component, match against part2 and return part1
                if (dPart2->unify(d)) {
                    // second part matches, add part1 to instantiations
                    instantiatons.append(dPart1);
                }
            } else {
                // d is first component, match against part1 and return part2
                if (dPart1->unify(d)) {
                    // first part matches, add part2 to instantiations
                    instantiatons.append(dPart2);
                }
            }
            break;
        case GNYDataType::ShareKey:
        case GNYDataType::ShareSecret:
            if (findFirstComponent) {
                // d is secret/key, find principals
                if (dPart2->unify(d)) {
                    // secret/key matches, add both principals to instantiations
                    instantiatons.append(dPart1);
                    instantiatons.append(dPart3);
                }
            } else {
                // d is principal, find key/secret
                if (dPart1->unify(d) || dPart3->unify(d)) {
                    // if either principal is matching, add secret
                    instantiatons.append(dPart2);
                }
            }
            break;
        default:
            throw UnrecognisedDataException("Unrecognised Data Type in GNY::Data::findData()");
            break;
        } // end switch (dType)
    } // end if (dType == dt)
    // now try to find in parts if parts are of suitable type
    if (dPart1 != nullptr) {
        if (dPart1->getComponentType() == GNYComponentType::Data) {
            d = dynamic_cast<Data *>(dPart1);
            switch (d->dType) {
            case GNYDataType::Atom:
            case GNYDataType::RecogniseData:
            case GNYDataType::FreshData:
            case GNYDataType::NotHereData:
            case GNYDataType::AnyData:
                break;
            case GNYDataType::DataData:
            case GNYDataType::DataK:
            case GNYDataType::DataPubK:
            case GNYDataType::DataPrivK:
            case GNYDataType::FData:
            case GNYDataType::HData:
            case GNYDataType::HasKey:
            case GNYDataType::DataExtendStmt:
            case GNYDataType::ShareKey:
            case GNYDataType::ShareSecret:
                dPart1->findData(c, dt, instantiatons, findFirstComponent, extend);
            }
        } else if (dPart1->getComponentType() == GNYComponentType::Statement){
            s = dynamic_cast<Statement *>(dPart1);
            if ( s->sType == GNYStatementType::PrnOpData || s->sType == GNYStatementType::PrnOpStatement) {
                dPart1->findData(c, dt, instantiatons, findFirstComponent, extend);
            }
        }
    }
    if (dPart2 != nullptr) {
        if (dPart2->getComponentType() == GNYComponentType::Data) {
            d = dynamic_cast<Data *>(dPart2);
            switch (d->dType) {
            case GNYDataType::Atom:
            case GNYDataType::RecogniseData:
            case GNYDataType::FreshData:
            case GNYDataType::NotHereData:
            case GNYDataType::AnyData:
                break;
            case GNYDataType::DataData:
            case GNYDataType::DataK:
            case GNYDataType::DataPubK:
            case GNYDataType::DataPrivK:
            case GNYDataType::FData:
            case GNYDataType::HData:
            case GNYDataType::HasKey:
            case GNYDataType::DataExtendStmt:
            case GNYDataType::ShareKey:
            case GNYDataType::ShareSecret:
                dPart2->findData(c, dt, instantiatons, findFirstComponent, extend);
            }
        } else if (dPart2->getComponentType() == GNYComponentType::Statement){
            s = dynamic_cast<Statement *>(dPart2);
            if ( s->sType == GNYStatementType::PrnOpData || s->sType == GNYStatementType::PrnOpStatement) {
                dPart2->findData(c, dt, instantiatons, findFirstComponent, extend);
            }
        }
    }
    if (dPart3 != nullptr) {
        if (dPart3->getComponentType() == GNYComponentType::Data) {
            d = dynamic_cast<Data *>(dPart1);
            switch (d->dType) {
            case GNYDataType::Atom:
            case GNYDataType::RecogniseData:
            case GNYDataType::FreshData:
            case GNYDataType::NotHereData:
            case GNYDataType::AnyData:
                break;
            case GNYDataType::DataData:
            case GNYDataType::DataK:
            case GNYDataType::DataPubK:
            case GNYDataType::DataPrivK:
            case GNYDataType::FData:
            case GNYDataType::HData:
            case GNYDataType::HasKey:
            case GNYDataType::DataExtendStmt:
            case GNYDataType::ShareKey:
            case GNYDataType::ShareSecret:
                dPart3->findData(c, dt, instantiatons, findFirstComponent, extend);
            }
        } else if (dPart3->getComponentType() == GNYComponentType::Statement){
            s = dynamic_cast<Statement *>(dPart3);
            if ( s->sType == GNYStatementType::PrnOpData || s->sType == GNYStatementType::PrnOpStatement) {
                dPart3->findData(c, dt, instantiatons, findFirstComponent, extend);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
/// instantiate(c)
/// instantiates the current object with object c
//////////////////////////////////////////////////////////////////////////
void Data::instantiate(GNYComponent *c)
{
    if (c->getComponentType() != GNYComponentType::Data) {
        throw IllegalComponentTypeException("illegal component type in GNY::Data::instantiate()");
    } else {
        Data *d = dynamic_cast<Data *>(c);
        // adjust dType if current is AnyData
        if (dType != d->dType && dType == GNYDataType::AnyData) {
            dType = d->dType;
        }
        // copy parts according to data type of passed object
        switch (d->dType) {
        case GNYDataType::Atom:
            dPart1 = new Atom(* dynamic_cast<Atom *>(d->dPart1));
            break;
        case GNYDataType::RecogniseData:
        case GNYDataType::FreshData:
        case GNYDataType::NotHereData:
            dPart1 = new Data(* dynamic_cast<Data *>(d->dPart1));
            break;
        case GNYDataType::DataData:
        case GNYDataType::DataK:
        case GNYDataType::DataPubK:
        case GNYDataType::DataPrivK:
        case GNYDataType::FData:
        case GNYDataType::HData:
        case GNYDataType::HasKey:
            dPart1 = new Data(* dynamic_cast<Data *>(d->dPart1));
            dPart2 = new Data(* dynamic_cast<Data *>(d->dPart2));
            break;
        case GNYDataType::DataExtendStmt:
            dPart1 = new Data(* dynamic_cast<Data *>(d->dPart1));
            dPart2 = new Statement(* dynamic_cast<Statement *>(d->dPart2));
            break;
        case GNYDataType::ShareKey:
        case GNYDataType::ShareSecret:
            dPart1 = new Data(* dynamic_cast<Data *>(d->dPart1));
            dPart2 = new Data(* dynamic_cast<Data *>(d->dPart2));
            dPart3 = new Data(* dynamic_cast<Data *>(d->dPart3));
            break;
        case GNYDataType::AnyData:
            // all parts are nullptr anyways - no need for anything
            break;
        default:
            throw UnrecognisedDataException("Unrecognised Data Type in GNY::Data::instantiate(c)");
            break;
        } // end switch (dType)


    }

}

//////////////////////////////////////////////////////////////////////////
/// returns string representation of current object
//////////////////////////////////////////////////////////////////////////
QString Data::getString()
{
    QString s = "";
    // string composition depends on data type
    switch(dType) {
    case GNYDataType::Atom:
        s.append(dPart1->getString());
        break;
    case GNYDataType::DataData:
        s.append(dPart1->getString() + ", " + dPart2->getString());
        break;
    case GNYDataType::DataK:
    case GNYDataType::DataPubK:
    case GNYDataType::DataPrivK:
        s.append(QString("{") + dPart1->getString() + "}" + dPart2->getString());
        break;
    case GNYDataType::FData:
    case GNYDataType::HData:
        s.append(dPart1->getString() + "(" + dPart2->getString() + ")");
        break;
    case GNYDataType::ShareSecret:
        s.append(dPart1->getString() + "<- <" + dPart2->getString() + "> ->" +dPart3->getString());
        break;
    case GNYDataType::ShareKey:
        s.append(dPart1->getString() + "<-" + dPart2->getString() + "->" +dPart3->getString());
        break;
    case GNYDataType::HasKey:
        s.append(dPart1->getString() + " has key " + dPart2->getString());
        break;
    case GNYDataType::RecogniseData:
        s.append(QString("recognises(") + dPart1->getString() + ")");
        break;
    case GNYDataType::FreshData:
        s.append(QString("#(") + dPart1->getString() + ")");
        break;
    case GNYDataType::NotHereData:
        s.append(QString("*(") + dPart1->getString() + ")");
        break;
    case GNYDataType::AnyData:
        s.append(dataID);
        break;
    case GNYDataType::DataExtendStmt:
        s.append(QString("(") + dPart1->getString() + ") extend (" + dPart2->getString() + ")");
        break;
    default:
        throw UnrecognisedDataException("Unrecognised Data in GNY::Data::getString()");
    }

    return s;
}

// unifies current component with c - c might get instantiated as consequence

// algorithm:
// if c is CStatement:
   //  unify is false as statement and data never unify
// else if c is Data
   // if dType equals c->dType
      // unify if parts according to type unify
   // else
      // unify if c->dtype is AnyData (requires instantiation c with this)
// else if c is Atom
   // unify if this has dtype Atom AND dPart1 unifies with c
bool Data::unify(GNYComponent *c)
{
    // assume true and falsify on evidence
    bool unifies = true;
    Data *d;

    if (c != nullptr) {
        switch (c->getComponentType()) {
        case GNYComponentType::Statement:
            // if c is CStatement:
               //  unify is false as statement and data never unify
            unifies = false;
            break;
        case GNYComponentType::Data:
            // else if c is Data
               // if dType equals c->dType
            d = dynamic_cast<Data *>(c);
            if (dType == d->dType) {
                  // unify if parts according to type unify
               switch (dType) {
               case GNYDataType::Atom:
               case GNYDataType::RecogniseData:
               case GNYDataType::FreshData:
               case GNYDataType::NotHereData:
                   unifies=dPart1->unify(d->dPart1);
                   break;
               case GNYDataType::DataData:
               case GNYDataType::DataK:
               case GNYDataType::DataPubK:
               case GNYDataType::DataPrivK:
               case GNYDataType::FData:
               case GNYDataType::HData:
               case GNYDataType::HasKey:
               case GNYDataType::DataExtendStmt:
                   unifies = (dPart1->unify(d->dPart1) &&
                              dPart2->unify(d->dPart2));
                   break;
               case GNYDataType::ShareKey:
               case GNYDataType::ShareSecret:
                   unifies = (dPart1->unify(d->dPart1) &&
                              dPart2->unify(d->dPart2) &&
                              dPart3->unify(d->dPart3));
                   break;
               case GNYDataType::AnyData:
                   throw IllegalDataTypeException("Illegal use of AnyData in GNY::Data::unify()");
                   break;
               default:
                   throw UnrecognisedDataException("Unrecognised Data Type in GNY::Data::unify()");
                   break;
               } // end switch (dType)
            } else { // if (dType == d->dType) { } else
               // else
                  // unify if c->dtype is AnyData (requires instantiation c with this)
                if (d->dType != GNYDataType::AnyData) {
                    unifies = false;
                } else {
                    d->instantiate(this);
                }
            }
            break;
        case GNYComponentType::Atom:
            // else if c is Atom
               // unify if this has dtype Atom AND dPart1 unifies with c
            if (dType == GNYDataType::Atom) {
                unifies = dPart1->unify(c);
            } else {
                unifies = false;
            }
            break;
        default:
            throw UnrecognisedComponentException("Unrecognised Component in GNY::Data::Unify()");

        }
    }

    return unifies;
}

// currently not used - not sure if there is any benefit in using it ....
bool Data::match(GNYComponent *c)
{
    return true;
}

}
