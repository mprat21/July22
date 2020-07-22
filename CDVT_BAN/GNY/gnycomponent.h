#ifndef GNYCOMPONENT_H
#define GNYCOMPONENT_H

#include "gnydefinitions.h"
#include "gnycontainer.h"

namespace GNY {

class GNYComponent
{
public:
    GNYComponent(GNYComponentType ct);
    virtual ~GNYComponent();
    virtual void findData(GNYComponent *d, GNYDataType dt, ComponentPtrList &instantiatons,
                          bool findFirstComponent=false, bool extend=false) const =0;
    virtual void instantiate(GNYComponent *c) =0;
    virtual QString getID() =0;
    virtual QString getString() =0;
    virtual void print() = 0;
    // unifies current component with c - c might get instantiated as consequence
    virtual bool unify(GNYComponent *c) =0;
    virtual bool match(GNYComponent *c) =0;
    GNYComponentType getComponentType() const { return componentType;}
    virtual bool operator==(const GNYComponent &other) =0;
protected:
    GNYComponentType componentType;
};

}
#endif // GNYCOMPONENT_H
