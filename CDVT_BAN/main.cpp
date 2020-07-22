#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

#include "LPT/LPTDefinitions.h"
#include "LPT/Layeredprovingtree.h"
#include "LPT/LPTContainers.h"

#include "GNY/gnydefinitions.h"
#include "GNY/atom.h"
#include "GNY/gnydata.h"

void test();


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}


void test()
{
    using namespace GNY;

    GNYAtom *a = new GNYAtom(GNYAtomType::Principal, "A");
    GNYAtom *b = new GNYAtom(GNYAtomType::Principal, "B");
    GNYAtom *n = new GNYAtom(GNYAtomType::Nonce, "Na");
    GNYAtom *k = new GNYAtom(GNYAtomType::SymKey, "Kab");

    GNYData *da = new GNYData(GNYDataType::Atom, a);
    GNYData *db = new GNYData(GNYDataType::Atom, b);
    GNYData *dn = new GNYData(GNYDataType::Atom, n);
    GNYData *dk = new GNYData(GNYDataType::Atom, k);

    GNYData *d1 = new GNYData(GNYDataType::DataData, da, dn);
    GNYData *d2 = new GNYData(GNYDataType::DataK, d1, dk);

    GNYAtom *kpriv = new GNYAtom(GNYAtomType::PrivKey, "A-");
    GNYData *dkpriv = new GNYData(GNYDataType::Atom, kpriv);

    GNYData *d3 = new GNYData(GNYDataType::DataPrivK, d2, dkpriv);

    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool", d3->getString());

    GNYData *d4 = new GNYData();
    d3->unify(d4);
    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool",
                         QString("d4 = ") + d4->getString() + "\nd3 = " + d3->getString());


    GNYStatement *s1 = new GNYStatement(GNYStatementType::PrnOpData, b, GNYOperatorType::possess, d3);
    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool",
                         QString("s1 = ") + s1->getString());

    GNYAtom *P = new GNYAtom(GNYAtomType::Principal);
    GNYData *anyD = new GNYData();

    GNYStatement *s2 = new GNYStatement(GNYStatementType::PrnOpData, P, GNYOperatorType::possess, anyD);

    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool",
                          QString("s2 = ") + s2->getString());
    if (!s1->unify(s2)) {
        QMessageBox::critical(nullptr, "Cryptographic-Protocol Development & Verification Tool", "s->unify(s2) failed");
    }
    QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool",
                          QString("s1 = ") + s1->getString() + "\ns2 = " + s2->getString());

}
