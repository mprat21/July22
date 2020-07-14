#ifndef STATEMENT_H
#define STATEMENT_H

#include <QString>

namespace LPT {
class Statement
{
public:
    Statement();
    virtual ~Statement();
    virtual bool operator==(const Statement& stmt)=0;
    virtual QString getString()=0;
private:
};

}

#endif // STATEMENT_H
