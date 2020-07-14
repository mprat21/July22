#include "banexception.h"
namespace BANLogic {

BanException::BanException()
{

}

BanException::BanException(QString str)
{
    what(str);
}

void BanException::raise() const
{
    throw *this;
}

QException *BanException::clone() const
{
    return new BanException(*this);
}


const void BanException::what(QString str) const noexcept
{
    QTextStream(stdout)<<str+" "<<endl;
}
}
