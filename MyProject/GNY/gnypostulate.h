#ifndef GNYPOSTULATE_H
#define GNYPOSTULATE_H

#include <QString>

#include "gnydefinitions.h"
#include "gnycontainer.h"
#include "gnystatement.h"
#include "../LPT/Postulate.h"

namespace GNY {

class Postulate : public LPT::Postulate
{
public:
    Postulate(GNYRuleID pid, LPT::Statement *pGoal, LPT::StatementPtrList &prereq);
    Postulate(Postulate &orig);
    virtual ~Postulate();
    inline virtual bool operator==(LPT::Postulate &p2) override;
    virtual LPT::Postulate *getCopy() override;
    virtual QString getString() override;

    void *operator new(std::size_t size);
    void operator delete(void *ptr);
    static void deleteAllDynamicPostulate();
    inline GNYRuleID getRuleID() {return id;}
    inline Statement *getGNYGoal() {return dynamic_cast<Statement *>(goal);}

protected:
    QString rule;
    GNYRuleID id;
    ComponentPtrList allAtomicComponents; // holds all Atoms and AnyData
    void updateAtomicComponents();
    static PostulatePtrList allDynamicPostulates;
};

}
#endif // GNYPOSTULATE_H
