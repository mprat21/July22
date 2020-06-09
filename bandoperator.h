#ifndef BANDOPERATOR_H
#define BANDOPERATOR_H
#include "bandcomponent.h"
namespace BANLogic{

class BanDOperator : public BanDComponent
{
protected:
    BanDOperatorType adtype;
    QString oValue;

public:
    BanDOperator();
    BanDOperator(BanDOperatorType adTy);
    virtual QString getID() override{return oValue;}
    virtual void print() override{QTextStream(stdout)<<this->oValue<<flush;}
    BanDOperatorType getDOtype() const;
    virtual void setDtype (QString dType) override;
    virtual bool match(BanDComponent *value) override;
    bool unify(BanDComponent *value) override;
    void setAdtype(const BanDOperatorType &value);
    bool getInstantiate() const override;
    void setId(const QString &value) override;
    bool getIfMatches() const override;
    virtual void printRPN() override{QTextStream(stdout)<<this->oValue<<flush;};

};
}
#endif // BANDOPERATOR_H
