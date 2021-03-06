#ifndef BANENUMTYPES_H
#define BANENUMTYPES_H
#include<iostream>
#include <QList>
#include <QStack>
#include <QString>
#include <QTextStream>
#include "banexception.h"
namespace BANLogic{

using namespace std;
class banEnums {
};

enum class BanDComponentType{bAtom, bOperator, bAnyData};

enum class BanSComponentType{bStatement,bData, bSOperator};

enum class BanDOperatorType {concates, Encryption,FreshData, HasKey,ShareKey,ShareSecret, SecretPassword} ;

enum class BanSOperatorType{said, possess, believes, controls,conveyed,sees} ;

typedef enum {Principal, SymKey, PubKey, PrivKey, Nonce, Timestamp, Function, Hash, BinaryData} banAtomtype;

typedef enum {PrnDataOp, PrnStatementOp} banStatementType;
enum class BanRuleId {I1,I2,I3,NV,J,
                      S1,S2,S3,S4,S5,
                      S6,S7,F1,F2,
                      B1,B2,B3,B4,
                      B5,B6,B7,B8} ;

const QString BanRuleNames[] = {"I1","I2","I3","NV","J",
                                "S1","S2","S3","S4","S5",
                                "S6","S7","F1","F2",
                                "B1","B2","B3","B4",
                                "B5","B6","B7","B8"};
/*
enum class AtomIds {_A_, _B_, _C_, _S_, _P_,
                     _Na_, _Nb_, _Nc_, _Ns_, _Np_,
                     _Ka_, _kb_, _Kc_, _Ks_,
                     _Kab_, _Kbc_, _Kac_, _Kas_, _Kbs_, _Kcs_,
                     _Ta_, _Tb_, _Tc_, _Ts_, _Tp_
                   };

enum class FreeDataIds {_D1_, _D2_, _D3_, _En_, _De_, _Key_};
*/
}
#endif // BANENUMTYPES_H
