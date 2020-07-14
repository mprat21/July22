#ifndef LPTCONTAINERS_H
#define LPTCONTAINERS_H

#include "LPTDefinitions.h"
#include "LPTList.h"
#include "LPTQueue.h"
#include "LPTStack.h"
//#include "Postulate.h"
//#include "Statement.h"


namespace LPT {

typedef LPTList<Postulate *> PostulatePtrList;
typedef LPTList<Statement *> StatementPtrList;

}
#endif // LPTCONTAINERS_H
