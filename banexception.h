#ifndef BANEXCEPTION_H
#define BANEXCEPTION_H
#include <QException>
#include<iostream>
#include <QTextStream>


class BanException: public QException
{
public:
    BanException();
    BanException(QString str);

    virtual void raise() const override;
    virtual QException *clone() const override;
    virtual const void what(QString str) const noexcept;
};

#endif // BANEXCEPTION_H
