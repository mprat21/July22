#ifndef GNYCONTAINER_H
#define GNYCONTAINER_H

#include "LPT/LPTDefinitions.h"
#include "LPT/LPTContainers.h"
#include "gnydefinitions.h"

namespace GNY {

typedef LPT::LPTPtrList<GNYComponent> ComponentPtrList;
typedef LPT::LPTPtrList<GNYAtom> AtomPtrList;
typedef LPT::LPTPtrList<GNYData> DataPtrList;
typedef LPT::LPTPtrList<GNYStatement> StatementPtrList;
typedef LPT::LPTPtrList<GNYPostulate> PostulatePtrList;

}
#endif // GNYCONTAINER_H
