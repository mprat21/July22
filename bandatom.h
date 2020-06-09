#ifndef BANATOM_H
#define BANATOM_H
#include "bandcomponent.h"
#include "QException"

namespace BANLogic{

class BanDAtom : public BanDComponent
{
protected:
    static int atomCount;
    banAtomtype atype;
    QString aValue;

public:
    BanDAtom();
    BanDAtom(banAtomtype aTy, QString aVal);
    virtual QString getID () override{return this->aValue;}
    virtual void print() override{QTextStream(stdout) <<this->getID()+" "<<flush;}
    virtual banAtomtype getAtype() const;
    virtual void setAtomtype(banAtomtype aTy);
    virtual void setDtype(QString aTy) override;
    virtual bool match(BanDComponent *value) override;
    bool unify(BanDComponent *value) override;
    void setId(const QString &value) override;
    bool getInstantiate() const override;
    bool getIfMatches() const override;
    virtual void printRPN()override{QTextStream(stdout) <<this->getID()+" "<<flush;};


};

}



#endif // BANATOM_H
