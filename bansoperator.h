#ifndef BANSOPERATOR_H
#define BANSOPERATOR_H
#include "banscomponent.h"
namespace BANLogic {

class BanSOperator: public BanSComponent
{
protected:
    BanSOperatorType astype;
    QString soValue;

public:
    BanSOperator();
    BanSOperator(BanSOperatorType adTy);
    // BanSComponent interface
    virtual QString getID() override{return soValue;}
    virtual void print() override{QTextStream(stdout)<<soValue<<flush;}
    void printRPN() override{QTextStream(stdout)<<"["+this->soValue+"]"<<flush;}
    BanSOperatorType getStOptype() const;
    virtual ~BanSOperator();
    bool getInstantiate () const override;
    bool match(BanSComponent &value) override;
    bool unify(BanSComponent &value) override;
    bool getIfMatches() const override;
    virtual void setId(const QString &value) override;
};

}
#endif // BANSOPERATOR_H
