#ifndef LOGIC_H
#define LOGIC_H

#include "LPTDefinitions.h"
#include "LPTContainers.h"

namespace LPT {
class Logic
{
public:
    Logic();
    virtual ~Logic();
    virtual bool getMatchingPostulates(Statement *stmt, PostulatePtrList &pl) = 0;
    virtual bool getAllInstantiatedPostulates(Postulate *p, PostulatePtrList &ipl) = 0;
    inline void setLPT(LayeredProvingTree *tree) { lpt = tree; }
protected:
    LayeredProvingTree *lpt;
};

}

#endif // LOGIC_H
