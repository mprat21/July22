#ifndef BANDCOMPONENTS_H
#define BANDCOMPONENTS_H
#include "banEnumTypes.h"
namespace BANLogic{

class BanDComponent
{
protected:
    BanDComponentType dtype;
    bool instantiate;
    bool unifies;
    bool ifMatches;
    QList<BanDComponent*> maindataList;

public:
    BanDComponent();
    BanDComponent(BanDComponentType dcTy);
    BanDComponentType getDtype() const;
    virtual void setDtype(QString dType)=0;
    virtual void setId(const QString &value)=0;
    virtual QString getID()=0;
    virtual void printRPN()=0;
    virtual void print() = 0;
    virtual bool getInstantiate() const=0;
    virtual bool match(BanDComponent *value)=0;
    virtual bool unify(BanDComponent *value)=0;
    virtual bool getIfMatches() const=0;
    virtual void setInstantiate(bool value)=0;
};
}


#endif // BANDCOMPONENT_H
