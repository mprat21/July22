#ifndef BANLOGICIMPL_H
#define BANLOGICIMPL_H
#include "bandatom.h"
#include "bansoperator.h"
#include "bandoperator.h"
#include "bandatalist.h"
#include "banstatementlist.h"
#include "banpostulates.h"


namespace BANLogic{

class BANLogicImpl
{
public:
    BANLogicImpl();
    void show();

private:
    QList<BanPostulates*> BANPostulates;
};
}
#endif // BANLOGICIMPL_H
