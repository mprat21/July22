#ifndef BANLOGICIMPL_H
#define BANLOGICIMPL_H
#include "bandatom.h"
#include "bansoperator.h"
#include "bandoperator.h"
#include "bandatalist.h"
#include "banstatementlist.h"
#include "banpostulates.h"
#include "bancontainer.h"

#include "../LPT/LPTDefinitions.h"
#include "../LPT/Logic.h"
#include "../LPT/Statement.h"
#include "../LPT/Postulate.h"
#include "../LPT/Layeredprovingtree.h"

#include <mainwindow.h>


namespace BANLogic{

class BANLogicImpl: public LPT::Logic
{
private:
    QList<BanPostulates*> BANPostulates;
    PostulatePtrList post;
    QTextBrowser *mycomment;
public:
    friend class BanDAtom;
    friend class BanDataList;
    friend class BanDataC;
    friend class BanPostulates;
    friend class BanLogicImpl;
    BANLogicImpl(){};
    BANLogicImpl(QTextBrowser *comment);
    void show();
    // Logic interface
    virtual bool getMatchingPostulates(LPT::Statement *stmt, LPT::PostulatePtrList &pl) override;
    virtual bool getAllInstantiatedPostulates(LPT::Postulate *p, LPT::PostulatePtrList &ipl) override;
    inline int getKnownTrueStatementCount() {return lpt->getKnownTrueStatementCount();}
    inline const BanStatementList *getKnownTrueStatement(int index) {return dynamic_cast<const BanStatementList *>(lpt->getKnownTrueStatement(index)); }
    QTextBrowser *getMycomment() const;
    void setMycomment(QTextBrowser *value);

};
}
#endif // BANLOGICIMPL_H
