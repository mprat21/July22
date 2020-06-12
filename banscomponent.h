#ifndef BANSCOMPONENT_H
#define BANSCOMPONENT_H
#include "banEnumTypes.h"
namespace BANLogic{

class BanSComponent
{
protected:
    BanSComponentType stype;
    bool instantiate;
    bool unifies;
    bool ifMatches;


public:
    BanSComponent();
    BanSComponent(BanSComponentType sTy);
    BanSComponent(QList<BanSComponent*> sList);

    BanSComponentType getStype() const;
    virtual void print()=0;
    virtual void printRPN() = 0;
    virtual QString getID()=0;
    virtual bool getInstantiate() const=0;
    virtual bool match(BanSComponent &value)=0;
    virtual bool unify(BanSComponent &value)=0;
    virtual bool getIfMatches() const=0;
};
}
#endif // BANSCOMPONENT_H
