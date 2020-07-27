#ifndef BANDATAC_H
#define BANDATAC_H
#include "bandatalist.h"
#include "bandcomponent.h"

namespace BANLogic{
class BanDataC:public BanDComponent
{
protected:
    static int dataCount;
    BanDComponentType type;
    QString dataValue;
    QList<BanDComponent*> myListdata;
    QStack<QString> printQStack;


public:
    friend class BanPostualtes;
    friend class BanDataC;
    friend class BanStatementList;
    friend class BanDataList;
    friend class BanDAtom;

    BanDataC();
    BanDataC(BanDataC &orig);
    ~BanDataC();

   // BanDataC(QList<BanDComponent*> dList1);
    virtual void setDtype(QString dType) override;
    virtual void setId(const QString &value) override;
    virtual QString getID() override;
    virtual void print() override;
    virtual bool getInstantiate() const override;
    virtual bool match(BanDComponent& value) override;
    virtual bool unify(BanDComponent& value) override;
    virtual bool getIfMatches() const override;
    QList<BanDComponent *> getMyListdata() const;
    virtual void printRPN()override;
    QStack<QString> getPrintQStack() const;
    virtual void setInstantiate(bool value) override{this->instantiate=value;}
    BanDataC* getCopy(LPT::LPTPtrList<BanDComponent> &components);


    // BanDComponent interface
public:
    virtual QString getString() override;
    virtual bool operator ==(const BanDComponent &other) override;
    virtual void instantiateObject(BanDComponent *other);

};
}
#endif // BANDATAC_H
