#ifndef DATA_H
#define DATA_H

#include <iostream>

#include "../LPT/LPTExceptions.h"
#include "gnydefinitions.h"
#include "gnycontainer.h"
#include "gnycomponent.h"
#include "gnystatement.h"
#include "atom.h"



namespace GNY {


class GNYData : public GNYComponent
{
    // GNYLogic, Atom, Statement need acess to private members (access methods would provide access to all classes)
    friend class GNYLogic;
    friend class GNYAtom;
    friend class GNYStatement;
    friend class GNYPostulate;

public:
    // methods specific to Data
    GNYData();
    GNYData(GNYDataType dt, GNYComponent *comp1=nullptr, GNYComponent *comp2=nullptr, GNYComponent *comp3=nullptr);
    GNYData(GNYData &orig);
    virtual ~GNYData();
    void *operator new(std::size_t size);
    void operator delete(void *ptr);
    static void deleteAllDynamicData();

    inline GNYDataType getDataType() {return dType;}
    GNYData *getCopy(ComponentPtrList &components);

    // methods inherited from GNYComponent
    virtual void findData(GNYComponent *c, GNYDataType dt, ComponentPtrList &instantiatons, bool findFirstComponent=false,
                          bool extend=false) const override;
    virtual void instantiate(GNYComponent *c) override;
    inline virtual QString getID() override {return dataID;}
    virtual QString getString() override;
    inline virtual void print() override { std::cout << getString().toStdString();}
    // unifies current component with c - c might get instantiated as consequence
    virtual bool unify(GNYComponent *c) override;
    virtual bool match(GNYComponent *c) override;
    virtual bool operator==(const GNYComponent &other) override;
protected:
    QString dataID;
    GNYDataType dType;
    GNYComponent *dPart1;
    GNYComponent *dPart2;
    GNYComponent *dPart3;

    static int dataCount;
    static ComponentPtrList allDynamicData;
};

}
#endif // DATA_H
