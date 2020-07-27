// LPTExceptions.h: defines exceptions that can be thrown by LPTs
//
//////////////////////////////////////////////////////////////////////

#ifndef LPTEXCEPTIONS_H
#define LPTEXCEPTIONS_H

#include <QString>


/////////////////////////////////////////////////////////////////////
// lptException
// generic exception class for LPT
// this should be used if no other exception is applicable
// further, this class forms the base-class for all other more
// specific exception classes
/////////////////////////////////////////////////////////////////////
class LPTException {
public:
    LPTException() { message = "Unspecified LPT Exception";}
    LPTException(QString msg) { message = msg;}
    inline QString getMessage() {return message;}

protected:
    QString message;
};

/////////////////////////////////////////////////////////////////////
// OutOfMemoryException
// exception thrown if new operator returns a NULL pointer
/////////////////////////////////////////////////////////////////////
class OutOfMemoryException : public LPTException {
public:
    OutOfMemoryException() {message = "Unspecified unrecognised outOfMemory Exception";}
    OutOfMemoryException(QString msg) : LPTException(msg){}
};

/////////////////////////////////////////////////////////////////////
// IllegalMethodException
// exception thrown if an illegal method is called on any object
/////////////////////////////////////////////////////////////////////
class IllegalMethodException : public LPTException {
public:
    IllegalMethodException() {message = "Unspecified unrecognised illegalMethod Exception";}
    IllegalMethodException(QString msg) : LPTException(msg){}
};

/////////////////////////////////////////////////////////////////////
// UnrecognisedAtomException
// exception thrown if an unrecognised atom is encountered within
// a message/statement component
/////////////////////////////////////////////////////////////////////
class UnrecognisedAtomException : public LPTException {
public:
    UnrecognisedAtomException() {message = "Unspecified unrecognised Atom Exception";}
    UnrecognisedAtomException(QString msg) : LPTException(msg){}
};

/////////////////////////////////////////////////////////////////////
// UnrecognisedDataException
// exception thrown if an unrecognised data item is encountered within
// a message/statement component
/////////////////////////////////////////////////////////////////////
class UnrecognisedDataException : public LPTException {
public:
    UnrecognisedDataException() {message = "Unspecified unrecognised Data Exception";}
    UnrecognisedDataException(QString msg) : LPTException(msg){}
};

/////////////////////////////////////////////////////////////////////
// UnrecognisedStatementException
// exception thrown if an unrecognised Statement item is encountered within
// a message/statement component
/////////////////////////////////////////////////////////////////////
class UnrecognisedStatementException : public LPTException {
public:
    UnrecognisedStatementException() {message = "Unspecified unrecognised Statement Exception";}
    UnrecognisedStatementException(QString msg) : LPTException(msg){}
};

/////////////////////////////////////////////////////////////////////
// UnrecognisedComponentException
// exception thrown if an unrecognised component item is encountered within
// a message/statement component
/////////////////////////////////////////////////////////////////////
class UnrecognisedComponentException : public LPTException {
public:
    UnrecognisedComponentException() {message = "Unspecified unrecognised Component Exception";}
    UnrecognisedComponentException(QString msg) : LPTException(msg){}
};

/////////////////////////////////////////////////////////////////////
// UnrecognisedPostulateException
// exception thrown if an unrecognised postulate item is encountered within
// a message/statement component
/////////////////////////////////////////////////////////////////////
class UnrecognisedPostulateException : public LPTException {
public:
    UnrecognisedPostulateException() {message = "Unspecified unrecognised Postulate Exception";}
    UnrecognisedPostulateException(QString msg) : LPTException(msg){}
};

/////////////////////////////////////////////////////////////////////
// IllegalComponentTypeException
// exception is thrown if an illegal component type is used in an operation
/////////////////////////////////////////////////////////////////////
class IllegalComponentTypeException : public LPTException {
public:
    IllegalComponentTypeException() {message = "Illegal component type";}
    IllegalComponentTypeException(QString msg) : LPTException(msg){}
};

/////////////////////////////////////////////////////////////////////
// IllegalDataTypeException
// exception is thrown if an illegal data type is used in an operation
/////////////////////////////////////////////////////////////////////
class IllegalDataTypeException : public LPTException {
public:
    IllegalDataTypeException() {message = "Illegal data type";}
    IllegalDataTypeException(QString msg) : LPTException(msg){}
};

/////////////////////////////////////////////////////////////////////
// IllegalStatementTypeException
// exception is thrown if an illegal statment type is used in an operation
/////////////////////////////////////////////////////////////////////
class IllegalStatementTypeException : public LPTException {
public:
    IllegalStatementTypeException() {message = "Illegal Statement type";}
    IllegalStatementTypeException(QString msg) : LPTException(msg){}
};

/*
/////////////////////////////////////////////////////////////////////
// illegalKeyException
// exception thrown if an illegal component is used as a key 
/////////////////////////////////////////////////////////////////////
class illegalKeyException : public lptException {
public:
    illegalKeyException() {std::strcpy(message, "Unspecified Illegal Key Exception");}
	illegalKeyException(char *msg) : lptException(msg){;}
};

/////////////////////////////////////////////////////////////////////
// illegalStatementTypeException
// exception thrown if an illegal statement type is encountered
/////////////////////////////////////////////////////////////////////
class illegalStatementTypeException : public lptException {
public:
    illegalStatementTypeException() {std::strcpy(message, "Unspecified Illegal Statement Type Exception");}
	illegalStatementTypeException(char *msg) : lptException(msg){;}
};

/////////////////////////////////////////////////////////////////////
// illegalStatementContextException
// exception thrown if an illegal statement context or combination of 
// contexts is encountered
/////////////////////////////////////////////////////////////////////
class illegalStatementContextException : public lptException {
public:
    illegalStatementContextException() {std::strcpy(message, "Unspecified Illegal Statement Context Exception");}
	illegalStatementContextException(char *msg) : lptException(msg){;}
};

/////////////////////////////////////////////////////////////////////
// illegalTimeException
// exception thrown if an illegal time is used (time < -1, or an 
// interval where end < start 
/////////////////////////////////////////////////////////////////////
class illegalTimeException : public lptException {
public:
    illegalTimeException() {std::strcpy(message, "Unspecified Illegal Time Exception");}
	illegalTimeException(char *msg) : lptException(msg){;}
};


/////////////////////////////////////////////////////////////////////
// unrecognisedDataException
// exception thrown if an unrecognised data item is encountered within
// a message/statement component
/////////////////////////////////////////////////////////////////////
class unrecognisedDataException : public lptException {
public:
    unrecognisedDataException() {std::strcpy(message, "Unspecified unrecognised Data Exception");}
	unrecognisedDataException(char *msg) : lptException(msg){;}
};

/////////////////////////////////////////////////////////////////////
// unrecognisedStatementException
// exception thrown if an unrecognised statement type is encountered within
// a message/statement component
/////////////////////////////////////////////////////////////////////
class unrecognisedStatementException : public lptException {
public:
    unrecognisedStatementException() {std::strcpy(message, "Unspecified unrecognised Statement Exception");}
	unrecognisedStatementException(char *msg) : lptException(msg){;}
};

/////////////////////////////////////////////////////////////////////
// unrecognisedPostulateException
// exception thrown if an unrecognised postulate type is encountered within
// a logic
/////////////////////////////////////////////////////////////////////
class unrecognisedPostulateException : public lptException {
public:
    unrecognisedPostulateException() {std::strcpy(message, "Unspecified unrecognised Postulate Exception");}
	unrecognisedPostulateException(char *msg) : lptException(msg){;}
};

/////////////////////////////////////////////////////////////////////
// unrecognisedComponentException
// exception thrown if an unrecognised component is encountered within
// a message/statement component
/////////////////////////////////////////////////////////////////////
class unrecognisedComponentException : public lptException {
public:
    unrecognisedComponentException() {std::strcpy(message, "Unspecified unrecognised Component Exception");}
	unrecognisedComponentException(char *msg) : lptException(msg){;}
};

/////////////////////////////////////////////////////////////////////
// unrecognisedOperatorException
// exception thrown if an unrecognised operator is encountered within
// a statement
/////////////////////////////////////////////////////////////////////
class unrecognisedOperatorException : public lptException {
public:
    unrecognisedOperatorException() {std::strcpy(message, "Unspecified unrecognised Operator Exception");}
	unrecognisedOperatorException(char *msg) : lptException(msg){;}
};


/////////////////////////////////////////////////////////////////////
// illegalStatementTypeExceptionInSteps
// 
/////////////////////////////////////////////////////////////////////
class illegalStatementTypeExceptionInSteps : public lptException {
public:
    illegalStatementTypeExceptionInSteps() {std::strcpy(message, "Illegal statement type - must be PrnOpTimeData!!!");}
	illegalStatementTypeExceptionInSteps(char *msg) : lptException(msg){;}
};

*/

#endif
