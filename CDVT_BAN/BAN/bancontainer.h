#ifndef BANCONTAINER_H
#define BANCONTAINER_H
#include "LPT/LPTDefinitions.h"
#include "LPT/LPTContainers.h"

#include "banEnumTypes.h"
#include "bandatalist.h"
#include "bandatom.h"
#include "banpostulates.h"
#include "banstatementlist.h"

namespace BANLogic {

typedef LPT::LPTList<BanDComponent*> ComponentPtrListD;
typedef LPT::LPTList<BanSComponent*> ComponentPtrListS;

typedef LPT::LPTList<BanDAtom *> AtomPtrList;
typedef LPT::LPTList<BanDataList *> DataPtrList;
typedef LPT::LPTList<BanStatementList *> StatementPtrList;
typedef LPT::LPTList<BanPostulates*> PostulatePtrList;

}
#endif // BANCONTAINER_H
