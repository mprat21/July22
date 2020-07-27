#ifndef GNYPOSTULATE_H
#define GNYPOSTULATE_H

#include <QString>

#include "gnydefinitions.h"
#include "gnycontainer.h"
#include "gnystatement.h"
#include "../LPT/Postulate.h"

namespace GNY {

class GNYPostulate : public LPT::Postulate
{
public:
    GNYPostulate(GNYRuleID pid, LPT::Statement *pGoal, LPT::StatementPtrList &prereq);
    GNYPostulate(GNYPostulate &orig);
    virtual ~GNYPostulate();
    inline virtual bool operator==(LPT::Postulate &p2) override;
    virtual LPT::Postulate *getCopy() override;
    virtual QString getString() override;

    void *operator new(std::size_t size);
    void operator delete(void *ptr);
    static void deleteAllDynamicPostulate();
    inline GNYRuleID getRuleID() {return id;}
    inline GNYStatement *getGNYGoal() {return dynamic_cast<GNYStatement *>(goal);}

protected:
    QString rule;
    GNYRuleID id;
    ComponentPtrList allAtomicComponents; // holds all Atoms and AnyData
    void updateAtomicComponents();
    static PostulatePtrList allDynamicPostulates;
};

}
#endif // GNYPOSTULATE_H
