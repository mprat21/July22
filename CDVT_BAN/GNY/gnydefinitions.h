#ifndef GNYDEFINITIONS_H
#define GNYDEFINITIONS_H

#include <QString>

namespace GNY {

// forward declarations of classes
class GNYComponent;
class GNYAtom;
class GNYData;
class GNYStatement;
class GNYPostulate;

enum class GNYAtomType {Principal, SymKey, PubKey, PrivKey, Nonce, Timestamp,
              Function, Hash, BinaryData};

enum class GNYOperatorType {told, possess, believe, hasJurisdiction,
              conveyed, nop} ;

const QString GNYOperatorNames[] = {"told", "possess", "believe", "hasJurisdition", "conveyed", "nop"};

// datatype atom is needed to encapsulate atoms in data. this allows to change
// data of type anydata to be "changed" to atom
enum class GNYDataType {Atom, DataData, DataK, DataPubK, DataPrivK, FData,
              HData, ShareSecret, ShareKey, HasKey, AnyData, RecogniseData,
              FreshData, NotHereData, DataExtendStmt} ;

enum class GNYStatementType {PrnOpData, PrnOpStatement, AnyStatement, AllStatements} ;

enum class GNYComponentType {Statement, Data, Atom} ;

enum class GNYRuleID {T1, T2a, T2b, T3, T4, T5a, T5b, T6,
              P1, P2a, P2b, P3, P3b, P4, P5, P5b, P6a, P6b, P7a, P7b, P8a, P8b,
              F1a, F1a2, F1b, F2a, F2b, F3a, F3b, F4a, F4b, F5, F6, F7a, F7b, F8, F9, F10, F11,
              R1a, R1a2, R1b, R2a, R2b, R3, R4, R5, R6,
              I1a, I1b, I1c, I2a, I2b, I2c, I3a, I3b, I4a, I4b, I5a, I5b, I6, I7, I7b,
              J1, J1b, J2, J2b, J3, J3b,
              Ex1, Ex2, Ex3, Ex4, Ex5, Ex6, Ex7, Ex8} ;

const QString GNYRuleNames[] = {"T1", "T2a", "T2b", "T3", "T4", "T5a", "T5b", "T6",
                                "P1", "P2a", "P2b", "P3", "P3b", "P4", "P5", "P5b", "P6a", "P6b", "P7a", "P7b", "P8a", "P8b",
                                "F1a", "F1a2", "F1b", "F2a", "F2b", "F3a", "F3b", "F4a", "F4b", "F5", "F6", "F7a", "F7b", "F8", "F9", "F10", "F11",
                                "R1a", "R1a2", "R1b", "R2a", "R2b", "R3", "R4", "R5", "R6",
                                "I1a", "I1b", "I1c", "I2a", "I2b", "I2c", "I3a", "I3b", "I4a", "I4b", "I5a", "I5b", "I6", "I7", "I7b",
                                "J1", "J1b", "J2", "J2b", "J3", "J3b",
                                "Ex1", "Ex2", "Ex3", "Ex4", "Ex5", "Ex6", "Ex7", "Ex8"};
}

#endif // GNYDEFINITIONS_H
