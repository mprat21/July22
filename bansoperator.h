#ifndef BANSOPERATOR_H
#define BANSOPERATOR_H
#include "banscomponent.h"
namespace BANLogic {

class banSOperator: public BanSComponent
{
protected:
    BanSOperatorType astype;
    QString soValue;

public:
    banSOperator();
    banSOperator(BanSOperatorType adTy);
    // BanSComponent interface
    virtual QString getID() override{return soValue;}
    virtual void print() override{QTextStream(stdout)<<soValue<<flush;}
    void printRPN();
    BanSOperatorType getStOptype() const;
    virtual ~banSOperator();
    bool getInstantiate () const override;
    bool match(BanSComponent &value) override;
    bool unify(BanSComponent &value) override;
    bool getIfMatches() const override;
};

}
#endif // BANSOPERATOR_H
