#ifndef ATOM_H
#define ATOM_H

#include <iostream>
#include <QString>

#include "gnydefinitions.h"
#include "gnycomponent.h"

#include "../LPT/LPTExceptions.h"


namespace GNY {

class Atom : public GNYComponent
{
public:
    friend class Data;
    friend class Statement;
    Atom(GNYAtomType at, QString identifier="");
    virtual ~Atom();
    void *operator new(std::size_t size);
    void operator delete(void *ptr);
    static void deleteAllDynamicAtoms();
    void setInverseKey(Atom *invKey);
    inline Atom *getInverseKey() { return inverseKey;}
    inline QString getString() override {return atomID; }
    virtual inline void findData(GNYComponent *d, GNYDataType dt, ComponentPtrList &instantiatons,
                                 bool findFirstComponent=false, bool extend=false) const override {;}
    void instantiate(GNYComponent *c) override;
    inline GNYAtomType returnAType() { return aType; }
    virtual inline void print() override { std::cout << atomID.toStdString() << std::flush;}
    virtual inline QString getID() override {return atomID;}
    virtual bool unify(GNYComponent *c) override;
    virtual bool match(GNYComponent *c) override;
    virtual bool operator==(const GNYComponent &other) override;
protected:
    GNYAtomType aType;
    QString atomID;
    bool instantiated;
    Atom *inverseKey;
    static int atomCount;
    static ComponentPtrList allDynamicAtoms;
};

}
#endif // ATOM_H
