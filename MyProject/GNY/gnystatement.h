#ifndef GNYSTATEMENT_H
#define GNYSTATEMENT_H

#include <QString>

#include "../LPT/LPTExceptions.h"
#include "../LPT/LPTDefinitions.h"
#include "../LPT/Statement.h"
#include "gnydefinitions.h"
#include "gnycomponent.h"
#include "gnyatom.h"
#include "gnydata.h"


namespace GNY {

class Statement: public GNYComponent, public LPT::Statement
{
public:
    friend class Atom;
    friend class Data;
    friend class Postulate;
    friend class GNYLogic;

    Statement();
    Statement(GNYStatementType type, Atom *principal, GNYOperatorType op, GNYComponent *part);
    Statement(Atom *principal, GNYOperatorType op, Data *part);
    Statement(Atom *principal, GNYOperatorType op, Statement *part);
    Statement(Statement &orig);
    virtual ~Statement();

    void *operator new(size_t size);
    void operator delete(void *ptr);
    void deleteAllDynamicStatement();
    Statement *getCopy(ComponentPtrList &components);

    virtual bool operator==(const LPT::Statement &stmt) override;

    virtual void findData(GNYComponent *s, GNYDataType dt, ComponentPtrList &instantiatons,
                          bool findFirstComponent=false, bool extend=false) const override;
    virtual void instantiate(GNYComponent *c) override;
    inline virtual QString getID() override {return statementID;}
    virtual QString getString() override ;
    virtual void print() override;
    // unifies current component with c - c might get instantiated as consequence
    virtual bool unify(GNYComponent *c) override;
    virtual bool match(GNYComponent *c) override;
    virtual bool operator==(const GNYComponent &other) override;
protected:
    QString string; // simplifies debugging
    GNYStatementType sType;
    Atom *sPrincipal;
    GNYOperatorType sOp;
    GNYComponent *sPart;
    QString statementID;
    static int statementCount;
    static ComponentPtrList allDynamicStatement;

};

}
#endif // GNYSTATEMENT_H
