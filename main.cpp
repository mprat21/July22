#include <QCoreApplication>
#include "bandatom.h"
#include "bansoperator.h"
#include "bandoperator.h"
#include "bandatalist.h"
#include "banstatementlist.h"
#include "banlogicimpl.h"
using namespace BANLogic;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    BANLogicImpl *mylogic=new BANLogicImpl();
    mylogic->show();
    return a.exec();
}
