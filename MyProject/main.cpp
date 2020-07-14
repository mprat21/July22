#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

#include "LPT/LPTDefinitions.h"
#include "LPT/Layeredprovingtree.h"
#include "LPT/LPTContainers.h"

#include "GNY/gnydefinitions.h"
#include "GNY/gnyatom.h"
#include "GNY/gnydata.h"

#include "BAN/banEnumTypes.h"
#include "BAN/bandatom.h"
#include "BAN/bandatalist.h"
#include "BAN/banlogicimpl.h"
#include "BAN/bandatac.h"

void testBAN();
void test();
void testVerify();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    try {
        // test();
        testBAN();
        //testVerify();
    } catch (LPTException lpte) {
        QMessageBox::critical(nullptr, "Cryptographic-Protocol Development & Verification Tool", "Unknown Exception!!!", lpte.getMessage());
    } catch (...) {
        QMessageBox::critical(nullptr, "Cryptographic-Protocol Development & Verification Tool", "Unknown Exception!!!");
    }

    MainWindow w;
    w.show();
    return a.exec();
}
void testBAN() {
    using namespace BANLogic;
    LPT::StatementPtrList assum;
    LPT::StatementPtrList step;
    LPT::StatementPtrList goals;

    BanDAtom *a = new BanDAtom(banAtomtype::Principal, "A");
    BanDAtom *b = new BanDAtom(banAtomtype::Principal, "B");
    BanDAtom *n = new BanDAtom(banAtomtype::Nonce, "Na");
    BanDAtom *k = new BanDAtom(banAtomtype::SymKey, "Kab");

    BanDAtom *da = a;
    BanDAtom *db = b;
    BanDAtom *dn = n;
    BanDAtom *dk = k;


    BanStatementList *a1 = new BanStatementList({new BanDataList({b}), new BanDataList({dk}),new BanSOperator(BanSOperatorType::possess)});
    assum.append(a1);
    BanDataList *d1 = new BanDataList({da, dn, new BanDOperator(BanDOperatorType::concates)});
    BanStatementList *s1 = new BanStatementList({new BanDataList({b}), d1, new BanSOperator(BanSOperatorType::said)});
    step.append(s1);
    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool", s1->getString());

    BanStatementList *g1 = new BanStatementList({new BanDataList({b}), new BanDataList({da}), new BanSOperator(BanSOperatorType::said)});

    BanStatementList *g2 = new BanStatementList({new BanDataList({b}), new BanDataList({dn}), new BanSOperator(BanSOperatorType::said)});

    goals.append(g1);
    goals.append(g2);

    LPT::LayeredProvingTree lpt(LPT::ImplementedLogics::GNY, assum, step, goals);

    lpt.printInfo();

    std::cout << "known statement = " << lpt.getKnownTrueStatementCount() << std::endl;
    lpt.verifyProtocol();
    std::cout << "known statement = " << lpt.getKnownTrueStatementCount() << std::endl;







/*
    BanDataList *d1 = new BanDataList({da, dn, new BanDOperator(BanDOperatorType::concates)});
    BanDataList *d2 = new BanDataList({da, dn, new BanDOperator(BanDOperatorType::concates), dk,new BanDOperator(BanDOperatorType::Encryption)});
    BanDAtom *kpriv = new BanDAtom(banAtomtype::PrivKey, "Ka-");

    BanDAtom *dkpriv = kpriv;

    BanDataList *d3 = new BanDataList({ da, dn, new BanDOperator(BanDOperatorType::concates), dk,new BanDOperator(BanDOperatorType::Encryption), dkpriv, new BanDOperator(BanDOperatorType::Encryption)});

    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool", d3->getString());

    BanDataList *d4 =new BanDataList({ new BanDataC()});
    d4->unify(*d3);
    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool",
                         QString("d4 = ") + d4->getString() + "\nd3 = " + d3->getString());


    BanStatementList *s1 = new BanStatementList({new BanDataList({b}), d3, new BanSOperator(BanSOperatorType::possess)});
    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool",
                         QString("s1 = ") + s1->getString());

    BanDAtom *P = new BanDAtom(banAtomtype::Principal,"P");
    BanDataC *anyD = new BanDataC();

    BanStatementList *s2 = new BanStatementList({new BanDataList({P}),new BanDataList({anyD}),new BanSOperator(BanSOperatorType::possess)} );

    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool",
                          QString("s2 = ") + s2->getString());
    if (!s2->unify(*s1)) {
        QMessageBox::critical(nullptr, "Cryptographic-Protocol Development & Verification Tool", "s->unify(s2) failed");
    }
    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool",
                          QString("s1 = ") + s1->getString() + "\ns2 = " + s2->getString());

  */
}

void testVerify() {
    using namespace GNY;

    LPT::StatementPtrList assum;
    LPT::StatementPtrList step;
    LPT::StatementPtrList goals;

    Atom *a = new Atom(GNYAtomType::Principal, "A");
    Atom *b = new Atom(GNYAtomType::Principal, "B");
    Atom *n = new Atom(GNYAtomType::Nonce, "Na");
    Atom *k = new Atom(GNYAtomType::SymKey, "Kab");

    Data *da = new Data(GNYDataType::Atom, a);
    Data *db = new Data(GNYDataType::Atom, b);
    Data *dn = new Data(GNYDataType::Atom, n);
    Data *dk = new Data(GNYDataType::Atom, k);


    Statement *a1 = new Statement(b, GNYOperatorType::possess, dk);
    assum.append(a1);
    Data *d1 = new Data(GNYDataType::DataData, da, dn);
    Statement *s1 = new Statement(b, GNYOperatorType::told, d1);
    step.append(s1);

    Statement *g1 = new Statement(b, GNYOperatorType::told, da);
    Statement *g2 = new Statement(b, GNYOperatorType::told, dn);
    goals.append(g1);
    goals.append(g2);

    LPT::LayeredProvingTree lpt(LPT::ImplementedLogics::GNY, assum, step, goals);

    lpt.printInfo();

    std::cout << "known statement = " << lpt.getKnownTrueStatementCount() << std::endl;
    lpt.verifyProtocol();
    std::cout << "known statement = " << lpt.getKnownTrueStatementCount() << std::endl;

}

void test()
{
    using namespace GNY;

    Atom *a = new Atom(GNYAtomType::Principal, "A");
    Atom *b = new Atom(GNYAtomType::Principal, "B");
    Atom *n = new Atom(GNYAtomType::Nonce, "Na");
    Atom *k = new Atom(GNYAtomType::SymKey, "Kab");

    Data *da = new Data(GNYDataType::Atom, a);
    Data *db = new Data(GNYDataType::Atom, b);
    Data *dn = new Data(GNYDataType::Atom, n);
    Data *dk = new Data(GNYDataType::Atom, k);

    Data *d1 = new Data(GNYDataType::DataData, da, dn);
    Data *d2 = new Data(GNYDataType::DataK, d1, dk);

    Atom *kpriv = new Atom(GNYAtomType::PrivKey, "A-");
    Data *dkpriv = new Data(GNYDataType::Atom, kpriv);

    Data *d3 = new Data(GNYDataType::DataPrivK, d2, dkpriv);

    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool", d3->getString());

    Data *d4 = new Data();
    d3->unify(d4);
    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool",
                         QString("d4 = ") + d4->getString() + "\nd3 = " + d3->getString());


    Statement *s1 = new Statement(GNYStatementType::PrnOpData, b, GNYOperatorType::possess, d3);
    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool",
                         QString("s1 = ") + s1->getString());

    Atom *P = new Atom(GNYAtomType::Principal);
    Data *anyD = new Data();

    Statement *s2 = new Statement(GNYStatementType::PrnOpData, P, GNYOperatorType::possess, anyD);

    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool",
                          QString("s2 = ") + s2->getString());
    if (!s1->unify(s2)) {
        QMessageBox::critical(nullptr, "Cryptographic-Protocol Development & Verification Tool", "s->unify(s2) failed");
    }
    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool",
                          QString("s1 = ") + s1->getString() + "\ns2 = " + s2->getString());

}
