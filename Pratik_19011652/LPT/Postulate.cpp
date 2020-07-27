#include "Postulate.h"

namespace LPT {
Postulate::Postulate()
{

}
Postulate::~Postulate()
{

}

StatementPtrList &Postulate::getPrerequisites()
{
    return prerequisites;
}

Statement * Postulate::getGoal()
{
    return goal;
}
}
