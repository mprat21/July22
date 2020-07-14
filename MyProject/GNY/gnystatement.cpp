#include <iostream>

#include "gnystatement.h"
#include "../LPT/LPTExceptions.h"

namespace GNY {

// define static members
int Statement::statementCount = 0;
ComponentPtrList Statement::allDynamicStatement;


//////////////////////////////////////////////////////////////////////////
/// default statement constructor - creates an object of type AnyStatement
//////////////////////////////////////////////////////////////////////////
Statement::Statement()
    :GNYComponent(GNYComponentType::Statement), sType(GNYStatementType::AnyStatement), sPrincipal(nullptr),
      sOp(GNYOperatorType::nop), sPart(nullptr)
{
    statementID = QString("S") + QString::number(statementCount++);
    string = getString();
}

//////////////////////////////////////////////////////////////////////////
/// default statement constructor - creates a generic statement with given details
//////////////////////////////////////////////////////////////////////////
Statement::Statement(GNYStatementType type, Atom *principal, GNYOperatorType op, GNYComponent *part)
    :GNYComponent(GNYComponentType::Statement), sType(type), sPrincipal(principal), sOp(op), sPart(part)
{
    statementID = QString("S") + QString::number(statementCount++);
    string = getString();
}

//////////////////////////////////////////////////////////////////////////
/// default statement constructor - creates an object of type AnyStatement
//////////////////////////////////////////////////////////////////////////
Statement::Statement(Atom *principal, GNYOperatorType op, Data *part)
    : GNYComponent(GNYComponentType::Statement), sType(GNYStatementType::PrnOpData), sPrincipal(principal), sOp(op), sPart(part)
{
    statementID = QString("S") + QString::number(statementCount++);
    string = getString();
}

Statement::Statement(Atom *principal, GNYOperatorType op, Statement *part)
    : GNYComponent(GNYComponentType::Statement), sType(GNYStatementType::PrnOpStatement), sPrincipal(principal), sOp(op), sPart(part)
{
    statementID = QString("S") + QString::number(statementCount++);
    string = getString();
}

Statement::Statement(Statement &orig)
    :GNYComponent(GNYComponentType::Statement), sType(orig.sType), sPrincipal(orig.sPrincipal),
      sOp(orig.sOp), sPart(nullptr)
{
    // statementID = QString("S") + QString::number(statementCount++); // reuse id from orig
    statementID = orig.statementID;
    instantiate(&orig);
    string = getString();
    //std::cout << "created " << getString().toStdString() << std::endl;

}

Statement::~Statement()
{
    // delete sPrincipal and sPart if they are not null
    if (sPrincipal != nullptr) delete sPrincipal;
    if (sPart != nullptr) delete sPart;
}

//////////////////////////////////////////////////////////////////////////
/// overloaded new operator ensures that objects allocated with new are
/// added to allDynamicAtoms
//////////////////////////////////////////////////////////////////////////
void *Statement::operator new(size_t size)
{
    void *p = malloc(size);
    // keep track of all dynamically created objects by adding p to allDynamicAtoms
    allDynamicStatement.append(static_cast<Statement *>(p));
    return p;
}

//////////////////////////////////////////////////////////////////////////
/// overloaded delete operator ensures that objects de-allocated with
/// delete are removed from allDynamicAtoms
//////////////////////////////////////////////////////////////////////////
void Statement::operator delete(void *ptr)
{
    allDynamicStatement.removeOne(static_cast<Data *>(ptr));
    free(ptr);
}

//////////////////////////////////////////////////////////////////////////
/// static method that deletes all remaining dynamic atom objects
//////////////////////////////////////////////////////////////////////////
void Statement::deleteAllDynamicStatement()
{
    // delete will also remove object from list
    for (int i=allDynamicStatement.count(); i>0; --i) {
        delete allDynamicStatement.at(i-1);
    }
}

Statement *Statement::getCopy(ComponentPtrList &components)
{
    Statement *copy = new Statement();
    Atom *a;
    Data *d;
    Data *d2;
    GNYComponent *c;
    copy->sType = sType;
    // search for sPrincipal in components
    for (int i=0; i<components.count(); ++i) {
        c = components.at(i);
        if (c->getComponentType() == GNYComponentType::Atom) {
            a = dynamic_cast<Atom *>(c);
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
            copy->sPart = dynamic_cast<Statement *>(sPart)->getCopy(components);
        } else {
            // sPart is Data
            d = dynamic_cast<Data *>(sPart);
            if (d->dType == GNYDataType::AnyData) {
                // find in components
                for (int i=0; i<components.count(); ++i) {
                    c = components.at(i);
                    if ( (c->getComponentType() == GNYComponentType::Data)
                         && *c == *d) {
                        copy->sPart = c;
                    }
                    if (c->getComponentType() == GNYComponentType::Data) {
                        d2 = dynamic_cast<Data *>(c);
                        if (d2->dataID == d->dataID) {
                            // found component, set it in copy
                            copy->sPart = d2;
                            break;
                        }
                    }
                }
            } else {
                // any other data requires copying
                copy->sPart = d->getCopy(components);
            }
        }
    }
    copy->statementID = statementID;
    return copy;
}

bool Statement::operator==(const GNYComponent &other)
{
    bool equals = true;
    if (other.getComponentType() != GNYComponentType::Statement) {
        equals = false;
    } else {
        const Statement *s = dynamic_cast<const Statement *>(&other);
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

bool Statement::operator==(const LPT::Statement &stmt)
{
    bool equals = true;
    const Statement *s = dynamic_cast<const Statement *>(&stmt);
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
void Statement::findData(GNYComponent *s, GNYDataType dt, ComponentPtrList &instantiatons,
                      bool findFirstComponent, bool extend) const
{
    Data *d;
    Statement *stmt;
    if (sPart != nullptr) {
        if (sPart->getComponentType() == GNYComponentType::Data) {
            d = dynamic_cast<Data *>(sPart);
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
                sPart->findData(sPart, dt, instantiatons, findFirstComponent, extend);
            }
        } else if (sPart->getComponentType() == GNYComponentType::Statement){
            stmt = dynamic_cast<Statement *>(sPart);
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
void Statement::instantiate(GNYComponent *c)
{
    // can only instantiate with other statement
    if (c->getComponentType() != GNYComponentType::Statement) {
        throw IllegalComponentTypeException("illegal component type in GNY::Statement::instantiate()");
    } else {
        Statement *s = dynamic_cast<Statement *>(c);
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
            sPrincipal = new Atom(*s->sPrincipal);
            sOp = s->sOp;
            if (s->sType == GNYStatementType::PrnOpData) {
                sPart = new Data(* dynamic_cast<Data *>(s->sPart));
            } else {
                sPart = new Statement(* dynamic_cast<Statement *>(s->sPart));
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
QString Statement::getString()
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
void Statement::print()
{
    std::cout << getString().toStdString() << std::endl;
}

//////////////////////////////////////////////////////////////////////////
/// unifies current component with c - c might get instantiated as consequence
//////////////////////////////////////////////////////////////////////////
bool Statement::unify(GNYComponent *c)
{
    bool unifies = true;
    Statement *s;
    switch(c->getComponentType()) {
    case GNYComponentType::Statement:
        s = dynamic_cast<Statement *>(c);
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


bool Statement::match(GNYComponent *c)
{
    return true;
}

}
