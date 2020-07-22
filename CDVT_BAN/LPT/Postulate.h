#ifndef POSTULATE_H
#define POSTULATE_H

#include <QString>

#include "LPTDefinitions.h"
#include "LPTContainers.h"

namespace LPT {

class Postulate
{
public:
    Postulate();
    virtual ~Postulate();
    virtual Postulate *getCopy() = 0;
    virtual QString getString() = 0;
    inline virtual QString getPostulateName() {return postulateName;}
    // intended only for two fully instantiated postulates
    virtual bool operator==(Postulate &p2)=0;
    virtual StatementPtrList &getPrerequisites();
    virtual Statement * getGoal();
protected:
    QString postulateName;
    StatementPtrList prerequisites;
    Statement *goal;
};

}
#endif // POSTULATE_H
