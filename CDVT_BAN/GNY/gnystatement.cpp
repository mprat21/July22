#include <iostream>

#include "gnystatement.h"
#include "../LPT/LPTExceptions.h"

namespace GNY {

// define static members
int GNYStatement::statementCount = 0;
ComponentPtrList GNYStatement::allDynamicStatement;


//////////////////////////////////////////////////////////////////////////
/// default statement constructor - creates an object of type AnyStatement
//////////////////////////////////////////////////////////////////////////
GNYStatement::GNYStatement()
    :GNYComponent(GNYComponentType::Statement), sType(GNYStatementType::AnyStatement), sPrincipal(nullptr),
      sOp(GNYOperatorType::nop), sPart(nullptr)
{
    statementID = QString("S") + QString::number(statementCount++);
    string = getString();
}

//////////////////////////////////////////////////////////////////////////
/// default statement constructor - creates a generic statement with given details
//////////////////////////////////////////////////////////////////////////
GNYStatement::GNYStatement(GNYStatementType type, GNYAtom *principal, GNYOperatorType op, GNYComponent *part)
    :GNYComponent(GNYComponentType::Statement), sType(type), sPrincipal(principal), sOp(op), sPart(part)
{
    statementID = QString("S") + QString::number(statementCount++);
    string = getString();
}

//////////////////////////////////////////////////////////////////////////
/// default statement constructor - creates an object of type AnyStatement
//////////////////////////////////////////////////////////////////////////
GNYStatement::GNYStatement(GNYAtom *principal, GNYOperatorType op, GNYData *part)
    : GNYComponent(GNYComponentType::Statement), sType(GNYStatementType::PrnOpData), sPrincipal(principal), sOp(op), sPart(part)
{
    statementID = QString("S") + QString::number(statementCount++);
    string = getString();
}

GNYStatement::GNYStatement(GNYAtom *principal, GNYOperatorType op, GNYStatement *part)
    : GNYComponent(GNYComponentType::Statement), sType(GNYStatementType::PrnOpStatement), sPrincipal(principal), sOp(op), sPart(part)
{
    statementID = QString("S") + QString::number(statementCount++);
    string = getString();
}

GNYStatement::GNYStatement(GNYStatement &orig)
    :GNYComponent(GNYComponentType::Statement), sType(orig.sType), sPrincipal(orig.sPrincipal),
      sOp(orig.sOp), sPart(nullptr)
{
    // statementID = QString("S") + QString::number(statementCount++); // reuse id from orig
    statementID = orig.statementID;
    instantiate(&orig);
    string = getString();
    //std::cout << "created " << getString().toStdString() << std::endl;

}

GNYStatement::~GNYStatement()
{
    // delete sPrincipal and sPart if they are not null
    if (sPrincipal != nullptr) delete sPrincipal;
    if (sPart != nullptr) delete sPart;
}

//////////////////////////////////////////////////////////////////////////
/// overloaded new operator ensures that objects allocated with new are
/// added to allDynamicAtoms
//////////////////////////////////////////////////////////////////////////
void *GNYStatement::operator new(size_t size)
{
    void *p = malloc(size);
    // keep track of all dynamically created objects by adding p to allDynamicAtoms
    allDynamicStatement.append(static_cast<GNYStatement *>(p));
    return p;
}

//////////////////////////////////////////////////////////////////////////
/// overloaded delete operator ensures that objects de-allocated with
/// delete are removed from allDynamicAtoms
//////////////////////////////////////////////////////////////////////////
void GNYStatement::operator delete(void *ptr)
{
    allDynamicStatement.removeOne(static_cast<GNYData *>(ptr));
    free(ptr);
}

//////////////////////////////////////////////////////////////////////////
/// static method that deletes all remaining dynamic atom objects
//////////////////////////////////////////////////////////////////////////
void GNYStatement::deleteAllDynamicStatement()
{
    // delete will also remove object from list
    for (int i=allDynamicStatement.count(); i>0; --i) {
        delete allDynamicStatement.at(i-1);
    }
}

GNYStatement *GNYStatement::getCopy(ComponentPtrList &components)
{
    GNYStatement *copy = new GNYStatement();
    GNYAtom *a;
    GNYData *d;
    GNYData *d2;
    GNYComponent *c;
    copy->sType = sType;
    // search for sPrincipal in components
    for (int i=0; i<components.count(); ++i) {
        c = components.at(i);
        if (c->getComponentType() == GNYComponentType::Atom) {
            a = dynamic_cast<GNYAtom *>(c);
            if (*a == *sPrincipal) {
                // found component, set it in copy
                copy->sPrincipal = a;
                break;
            }
        }
    }
    copy->sOp = sOp;
    if (sPart != nullptr) {
        if (sPart->getComponentType() == GNYComponentType::Statement) {
            copy->sPart = dynamic_cast<GNYStatement *>(sPart)->getCopy(components);
        } else {
            // sPart is Data
            d = dynamic_cast<GNYData *>(sPart);
            // if d->dType is anydata find it in components
            // otherwise use copy constructor for sPart
            if (d->dType == GNYDataType::AnyData) {
                // find in components
                for (int i=0; i<components.count(); ++i) {
                    c = components.at(i);
                    if ( (c->getComponentType() == GNYComponentType::Data)  && *c == *d) {
                        copy->sPart = c;
                        break;
                    }
                }
            } else {
                // any other data (different to anydata) requires copying
                copy->sPart = d->getCopy(components);
            }
        }
    }
    copy->statementID = statementID;
    return copy;
}

bool GNYStatement::operator==(const GNYComponent &other)
{
    bool equals = true;
    if (other.getComponentType() != GNYComponentType::Statement) {
        equals = false;
    } else {
        const GNYStatement *s = dynamic_cast<const GNYStatement *>(&other);
        // copy parts according to statment type of passed object
        switch (sType) {
        case GNYStatementType::AllStatements:
        case GNYStatementType::AnyStatement:
            equals = sType == s->sType;
            break;
        case GNYStatementType::PrnOpData:
        case GNYStatementType::PrnOpStatement:
            equals = sType == s->sType &&
                    *sPrincipal == *(s->sPrincipal) &&
                    sOp == s->sOp &&
                    *(sPart) == *(s->sPart);
            break;
        default:
            throw UnrecognisedStatementException("Unrecognised StatementType in GNY::Statement::operator==()");
        }
    }
    return equals;
}

bool GNYStatement::operator==(const LPT::Statement &stmt)
{
    bool equals = true;
    const GNYStatement *s = dynamic_cast<const GNYStatement *>(&stmt);
    if (s == nullptr) {
        // if passed stmt is not GNY statement, comparison fails
        equals = false;
    } else {
        equals = *this == dynamic_cast<const GNYComponent &>(stmt);
    }
    return equals;
}

//////////////////////////////////////////////////////////////////////////
/// this is really only used on data - thus, traverse into sPart (if exists) to search there.
//////////////////////////////////////////////////////////////////////////
void GNYStatement::findData(GNYComponent *s, GNYDataType dt, ComponentPtrList &instantiatons,
                      bool findFirstComponent, bool extend) const
{
    GNYData *d;
    GNYStatement *stmt;
    if (sPart != nullptr) {
        if (sPart->getComponentType() == GNYComponentType::Data) {
            d = dynamic_cast<GNYData *>(sPart);
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
                sPart->findData(s, dt, instantiatons, findFirstComponent, extend);
            }
        } else if (sPart->getComponentType() == GNYComponentType::Statement){
            stmt = dynamic_cast<GNYStatement *>(sPart);
            if ( stmt->sType == GNYStatementType::PrnOpData || stmt->sType == GNYStatementType::PrnOpStatement) {
                sPart->findData(s, dt, instantiatons, findFirstComponent, extend);
           }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
/// instantiate(c)
/// instantiates the current object with object c
//////////////////////////////////////////////////////////////////////////
void GNYStatement::instantiate(GNYComponent *c)
{
    // can only instantiate with other statement
    if (c->getComponentType() != GNYComponentType::Statement) {
        throw IllegalComponentTypeException("illegal component type in GNY::Statement::instantiate()");
    } else {
        GNYStatement *s = dynamic_cast<GNYStatement *>(c);
        // copy parts according to statment type of passed object
        switch (s->sType) {
        case GNYStatementType::AllStatements:
        case GNYStatementType::AnyStatement:
            sType = s->sType;
            sPrincipal = nullptr;
            sOp = GNYOperatorType::nop;
            sPart = nullptr;
            // no need to delete sPrincipal/sPart - they are kept in alldynamic lists and will be deleted
            // eventually
            break;
        case GNYStatementType::PrnOpData:
        case GNYStatementType::PrnOpStatement:
            sType = s->sType;
            sPrincipal = new GNYAtom(*s->sPrincipal);
            sOp = s->sOp;
            if (s->sType == GNYStatementType::PrnOpData) {
                sPart = new GNYData(* dynamic_cast<GNYData *>(s->sPart));
            } else {
                sPart = new GNYStatement(* dynamic_cast<GNYStatement *>(s->sPart));
            }
            break;
        default:
            throw UnrecognisedStatementException("Unrecognised StatementType in GNY::Statement::instantiate()");
        }
    }
    string = getString();
}

//////////////////////////////////////////////////////////////////////////
/// returns string representation of Statement
//////////////////////////////////////////////////////////////////////////
QString GNYStatement::getString()
{
   QString s;
   switch (sType) {
   case GNYStatementType::AllStatements:
       s.append("*");
       break;
   case GNYStatementType::AnyStatement:
       s.append("C");
       break;
   case GNYStatementType::PrnOpData:
   case GNYStatementType::PrnOpStatement:
       s.append(sPrincipal->getString() + " ");
       s.append(GNYOperatorNames[static_cast<int>(sOp)] + " ");
       s.append(sPart->getString());
       break;
   default:
       throw UnrecognisedStatementException("Unrecognised StatementType in GNY::Statement::getString()");
   }
   return s;
}

//////////////////////////////////////////////////////////////////////////
/// prints statement to cout
//////////////////////////////////////////////////////////////////////////
void GNYStatement::print()
{
    std::cout << getString().toStdString() << std::endl;
}

//////////////////////////////////////////////////////////////////////////
/// unifies current component with c - c might get instantiated as consequence
//////////////////////////////////////////////////////////////////////////
bool GNYStatement::unify(GNYComponent *c)
{
    bool unifies = true;
    GNYStatement *s;
    switch(c->getComponentType()) {
    case GNYComponentType::Statement:
        s = dynamic_cast<GNYStatement *>(c);
        if (s->sType == GNYStatementType::AnyStatement) {
            s->instantiate(this);
        } else {
            if (sType == s->sType) {
               // unifies if parts unify
                switch(sType) {
                case GNYStatementType::AnyStatement:
                    throw IllegalStatementTypeException("Illegal use of AnyStatement in GNY::Statement::unify");
                    break;
                case GNYStatementType::AllStatements:
                    // only unifies with AllStatements
                    unifies = s->sType == GNYStatementType::AllStatements;
                    break;
                case GNYStatementType::PrnOpData:
                case GNYStatementType::PrnOpStatement:
                    // unifies if all components unify
                    if (!sPrincipal->unify(s->sPrincipal)) {
                        unifies = false;
                    } else if (sOp != s->sOp) {
                        unifies = false;
                    } else {
                        if (s->sPart != nullptr) {
                            unifies = sPart->unify(s->sPart);
                        } else {
                            unifies = false;
                        }
                    }
                    break;
                default:
                    throw UnrecognisedStatementException("Unrecognised Statement Exception in GNY::Statement::unify()");
                }
            }
        }
        break;
    case GNYComponentType::Atom:
    case GNYComponentType::Data:
        // Statement never unifies with Atom or Data
        unifies = false;
        break;
    default:
        throw UnrecognisedComponentException("Unrecognised Component Exception in GNY::Statement::unify()");
    }
    return unifies;
}


bool GNYStatement::match(GNYComponent *c)
{
    return true;
}

}
