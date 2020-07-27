#ifndef GNYSTATEMENT_H
#define GNYSTATEMENT_H

#include <QString>

#include "../LPT/LPTExceptions.h"
#include "../LPT/LPTDefinitions.h"
#include "../LPT/Statement.h"
#include "gnydefinitions.h"
#include "gnycomponent.h"
#include "atom.h"
#include "gnydata.h"


namespace GNY {

class GNYStatement: public GNYComponent, public LPT::Statement
{
public:
    friend class GNYAtom;
    friend class GNYData;
    friend class GNYPostulate;
    friend class GNYLogic;

    GNYStatement();
    GNYStatement(GNYStatementType type, GNYAtom *principal, GNYOperatorType op, GNYComponent *part);
    GNYStatement(GNYAtom *principal, GNYOperatorType op, GNYData *part);
    GNYStatement(GNYAtom *principal, GNYOperatorType op, GNYStatement *part);
    GNYStatement(GNYStatement &orig);
    virtual ~GNYStatement();

    void *operator new(size_t size);
    void operator delete(void *ptr);
    void deleteAllDynamicStatement();
    GNYStatement *getCopy(ComponentPtrList &components);

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
    GNYAtom *sPrincipal;
    GNYOperatorType sOp;
    GNYComponent *sPart;
    QString statementID;
    static int statementCount;
    static ComponentPtrList allDynamicStatement;

};

}
#endif // GNYSTATEMENT_H
