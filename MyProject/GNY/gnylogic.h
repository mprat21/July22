#ifndef GNYLOGIC_H
#define GNYLOGIC_H

#include "../LPT/LPTDefinitions.h"
#include "../LPT/Logic.h"
#include "../LPT/Statement.h"
#include "../LPT/Postulate.h"
#include "../LPT/Layeredprovingtree.h"

#include "gnydefinitions.h"
#include "gnystatement.h"

namespace GNY {

class GNYLogic : public LPT::Logic
{
public:

    GNYLogic();
    virtual bool getMatchingPostulates(LPT::Statement *stmt, LPT::PostulatePtrList &pl) override;
    virtual bool getAllInstantiatedPostulates(LPT::Postulate *p, LPT::PostulatePtrList &ipl) override;

    inline int getKnownTrueStatementCount() {return lpt->getKnownTrueStatementCount();}
    inline const Statement *getKnownTrueStatement(int index) {return dynamic_cast<const Statement *>(lpt->getKnownTrueStatement(index)); }

private:
    PostulatePtrList postulates;
};

} // end namespace GNY
#endif // GNYLOGIC_H
