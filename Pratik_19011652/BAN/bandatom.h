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
    BanDAtom *inverseKey;
    static QList<BanDComponent*> &allAtom;



public:
    friend class BanPostualtes;
    friend class BanDataC;
    friend class BanStatementList;
    friend class BanDataList;
    BanDAtom();
    BanDAtom(banAtomtype aTy, QString aVal);
    BanDAtom(BanDAtom &orig);
    ~BanDAtom();
    virtual void setAtomtype(banAtomtype aTy);
    virtual void setDtype(QString aTy) override;
    virtual void setInstantiate(bool value) override{this->instantiate=value;}
    void setInverseKey(BanDAtom *value);
    void setId(const QString &value) override;
    virtual QString getID () override{return this->aValue;}
    virtual banAtomtype getAtype() const;
    BanDAtom *getInverseKey() const;
    bool getInstantiate() const override;
    bool getIfMatches() const override;
    virtual bool match(BanDComponent& value) override;
    bool unify(BanDComponent& value) override;
    virtual void print() override{QTextStream(stdout) <<" "+this->getID()+" "<<flush;}
    virtual void printRPN()override{QTextStream(stdout) <<" "+this->getID()+" "<<flush;};

    // BanDComponent interface
    virtual QString getString() override;
    virtual bool operator ==(const BanDComponent &other) override;
    virtual void instantiateObject(BanDComponent *other);

};

}



#endif // BANATOM_H
