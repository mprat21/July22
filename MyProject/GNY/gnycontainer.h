#ifndef GNYCONTAINER_H
#define GNYCONTAINER_H

#include "LPT/LPTDefinitions.h"
#include "LPT/LPTContainers.h"
#include "gnydefinitions.h"

namespace GNY {

typedef LPT::LPTList<GNYComponent *> ComponentPtrList;
typedef LPT::LPTList<Atom *> AtomPtrList;
typedef LPT::LPTList<Data *> DataPtrList;
typedef LPT::LPTList<Statement *> StatementPtrList;
typedef LPT::LPTList<Postulate *> PostulatePtrList;

}
#endif // GNYCONTAINER_H
