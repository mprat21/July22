#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QMessageBox>
//#include <QScrollBarPolicy>
#include <iostream>

#include "LPT/Layeredprovingtree.h"
#include "LPT/LPTContainers.h"

#include "GNY/gnydefinitions.h"
#include "GNY/atom.h"
#include "GNY/gnydata.h"

#include "BAN/banEnumTypes.h"
#include "BAN/bandatom.h"
#include "BAN/bandatalist.h"
#include "BAN/banlogicimpl.h"
#include "BAN/bandatac.h"

LPT::LayeredProvingTree * testVerify(QTextBrowser *comment);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // enable horizontal scroll bar in tree view to appear when needed
    ui->setupUi(this);
    ui->treeView->header()->setStretchLastSection(false);
    ui->treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_MainWindow_iconSizeChanged(const QSize &iconSize)
{

}

void MainWindow::startVerification()
{
    QStandardItemModel *model = new QStandardItemModel;
    model->setHorizontalHeaderItem(0, new QStandardItem("Verification"));
    QStandardItem *parentItem = model->invisibleRootItem();
    QStandardItem *assum = new QStandardItem("1. Protocol Assumptions");
    parentItem->appendRow(assum);
    QStandardItem *steps = new QStandardItem("2. Protocol Steps");
    parentItem->appendRow(steps);
    QStandardItem *goals = new QStandardItem("3. Protocol Goals");
    parentItem->appendRow(goals);

    try {
        ui->comments->setText(QString("Verification Comments:\n"));
        lpt = testVerify(ui->comments);
        ;
    } catch (LPTException lpte) {
        QMessageBox::critical(nullptr, "Cryptographic-Protocol Development & Verification Tool", "Unknown Exception!!!", lpte.getMessage());
    } catch (...) {
        QMessageBox::critical(nullptr, "Cryptographic-Protocol Development & Verification Tool", "Unknown Exception!!!");
    }
    lpt->addResultsToModel(model);

    ui->treeView->setModel(model);
    //ui->treeView->expandAll();

}

void MainWindow::on_MainWindow_tabifiedDockWidgetActivated(QDockWidget *dockWidget)
{

}

void MainWindow::on_MainWindow_toolButtonStyleChanged(const Qt::ToolButtonStyle &toolButtonStyle)
{

}


/*
LPT::LayeredProvingTree *testVerify(QTextBrowser *comment) {
    using namespace GNY;

    LPT::StatementPtrList assum;
    LPT::StatementPtrList step;
    LPT::StatementPtrList goals;

    GNYAtom *a = new GNYAtom(GNYAtomType::Principal, "A");
    GNYAtom *b = new GNYAtom(GNYAtomType::Principal, "B");
    GNYAtom *na = new GNYAtom(GNYAtomType::Nonce, "Na");
    GNYAtom *nb = new GNYAtom(GNYAtomType::Nonce, "Nb");
    GNYAtom *na2 = new GNYAtom(GNYAtomType::Nonce, "Na2");
    GNYAtom *kab = new GNYAtom(GNYAtomType::SymKey, "Kab");

    GNYData *da = new GNYData(GNYDataType::Atom, a);
    GNYData *db = new GNYData(GNYDataType::Atom, b);
    GNYData *dna = new GNYData(GNYDataType::Atom, na);
    GNYData *dnb = new GNYData(GNYDataType::Atom, nb);
    GNYData *dna2 = new GNYData(GNYDataType::Atom, na2);
    GNYData *dkab = new GNYData(GNYDataType::Atom, kab);


    GNYStatement *a1 = new GNYStatement(b, GNYOperatorType::possess, dkab);
    assum.append(a1);
    GNYStatement *s1 = new GNYStatement(b, GNYOperatorType::told, dna);
    step.append(s1);
    GNYData *d1 = new GNYData(GNYDataType::DataData, da, dnb);
    GNYStatement *s2 = new GNYStatement(b, GNYOperatorType::told, d1);
    step.append(s2);
    d1 = new GNYData(GNYDataType::DataK, dna2, dkab);
    s1 = new GNYStatement(b, GNYOperatorType::told, d1);
    step.append(s1);

    GNYStatement *g1 = new GNYStatement(b, GNYOperatorType::possess, dna);
    GNYStatement *g2 = new GNYStatement(b, GNYOperatorType::told, da);
    GNYStatement *g3 = new GNYStatement(b, GNYOperatorType::told, dnb);
    goals.append(g1);
    goals.append(g2);
    goals.append(g3);
    GNYStatement *g4 = new GNYStatement(b, GNYOperatorType::told, dna2);
    goals.append(g4);

    LPT::LayeredProvingTree *lpt = new LPT::LayeredProvingTree(LPT::ImplementedLogics::GNY, assum, step, goals, comment);

    lpt->printInfo();

    lpt->verifyProtocol();

    return lpt;
}
*/
LPT::LayeredProvingTree *testVerify(QTextBrowser *comment) {
    using namespace BANLogic;
    LPT::StatementPtrList assum;
    LPT::StatementPtrList step;
    LPT::StatementPtrList goals;
    //YAHALOM Protocol
    BanDAtom *A = new BanDAtom(banAtomtype::Principal, "A");
    BanDAtom *S = new BanDAtom(banAtomtype::Principal, "S");
    BanDAtom *B = new BanDAtom(banAtomtype::Principal, "B");
    BanDAtom *Kas = new BanDAtom(banAtomtype::SymKey, "Kas");
    BanDAtom *Kbs = new BanDAtom(banAtomtype::SymKey, "Kbs");
    BanDAtom *Kab = new BanDAtom(banAtomtype::SymKey, "Kab");
    BanDAtom *K = new BanDAtom(banAtomtype::SymKey, "");
    BanDAtom *Na = new BanDAtom(banAtomtype::Nonce, "Na");
    BanDAtom *Nb = new BanDAtom(banAtomtype::Nonce, "Nb");
    //Assumptions:
    BanStatementList *a1 = new BanStatementList({
                                                    new BanDataList({A}),
                                                    new BanDataList({A,S,Kas,new BanDOperator(BanDOperatorType::ShareKey)}),
                                                    new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *a2 = new BanStatementList({
                                                    new BanDataList({S}),
                                                    new BanDataList({A,S,Kas,new BanDOperator(BanDOperatorType::ShareKey)}),
                                                    new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *a3 = new BanStatementList({
                                                    new BanDataList({B}),
                                                    new BanDataList({B,S,Kbs,new BanDOperator(BanDOperatorType::ShareKey)}),
                                                    new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *a4 = new BanStatementList({
                                                    new BanDataList({S}),
                                                    new BanDataList({B,S,Kbs,new BanDOperator(BanDOperatorType::ShareKey)}),
                                                    new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *a5 = new BanStatementList({
                                                    new BanDataList({S}),
                                                    new BanDataList({A,B,Kab,new BanDOperator(BanDOperatorType::ShareKey)}),
                                                    new BanSOperator(BanSOperatorType::believes)});

    BanStatementList *temp1 = new BanStatementList({
                                                       new BanDataList({S}),
                                                       new BanDataList({A,B,K,new BanDOperator(BanDOperatorType::ShareKey)}),
                                                       new BanSOperator(BanSOperatorType::controls)});
    BanStatementList *a6 = new BanStatementList({
                                                    new BanDataList({A}),
                                                    temp1,
                                                    new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *a7 = new BanStatementList({
                                                    new BanDataList({B}),
                                                    temp1,
                                                    new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *a8 = new BanStatementList({
                                                    new BanDataList({A}),
                                                    new BanDataList({Na,new BanDOperator(BanDOperatorType::FreshData)}),
                                                    new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *a9 = new BanStatementList({
                                                    new BanDataList({B}),
                                                    new BanDataList({Nb,new BanDOperator(BanDOperatorType::FreshData)}),
                                                    new BanSOperator(BanSOperatorType::believes)});

    //Steps:
    BanStatementList *s1 = new BanStatementList({new BanDataList({S}),
                                                 new BanDataList({A,B,Kab,new BanDOperator(BanDOperatorType::ShareKey),new BanDOperator(BanDOperatorType::FreshData)}),
                                                 new BanSOperator(BanSOperatorType::believes)});

    BanStatementList *temp2 = new BanStatementList({new BanDataList({S}),
                                                    new BanDataList({A,B,Kab,new BanDOperator(BanDOperatorType::ShareKey),new BanDOperator(BanDOperatorType::FreshData)}),
                                                    new BanSOperator(BanSOperatorType::controls)});

    BanStatementList *s2 = new BanStatementList({new BanDataList({B}),
                                                 temp2,
                                                 new BanSOperator(BanSOperatorType::believes)});

    BanStatementList *s3 = new BanStatementList({new BanDataList({B}),
                                                 new BanDataList({A,B,Nb,new BanDOperator(BanDOperatorType::ShareSecret)}),
                                                 new BanSOperator(BanSOperatorType::believes)});

    BanStatementList *temp3 = new BanStatementList({new BanDataList({A}),
                                                    s1,
                                                    new BanSOperator(BanSOperatorType::controls)});

    BanStatementList *s4 = new BanStatementList({
                                                    new BanDataList({B}),
                                                    temp3,
                                                    new BanSOperator(BanSOperatorType::believes)
                                                });

    BanStatementList *dummy1 = new BanStatementList({
                                                        new BanDataList({A}),
                                                        new BanDataList({Na,B,new BanDOperator(BanDOperatorType::concates)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });

    //Goals:
    BanStatementList *g1 = new BanStatementList({
                                                    new BanDataList({A}),
                                                    new BanDataList({A,B,Kab,new BanDOperator(BanDOperatorType::ShareKey)}),
                                                    new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *g2 = new BanStatementList({
                                                    new BanDataList({B}),
                                                    new BanDataList({A,B,Kab,new BanDOperator(BanDOperatorType::ShareKey)}),
                                                    new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *g3 = new BanStatementList({
                                                    new BanDataList({B}),
                                                    g1,
                                                    new BanSOperator(BanSOperatorType::believes)});

    BanStatementList *g4 = new BanStatementList({
                                                    new BanDataList({A}),
                                                    g2 ,
                                                    new BanSOperator(BanSOperatorType::believes)});



    BanStatementList *dummy2 = new BanStatementList({new BanDataList({A}),new BanDataList({Na,Nb,new BanDOperator(BanDOperatorType::concates),new BanDOperator(BanDOperatorType::FreshData)}),new BanSOperator(BanSOperatorType::believes)});

    BanStatementList *dummy3 = new BanStatementList({new BanDataList({A}),new BanDataList({B}),new BanDataList({Nb}),new BanSOperator(BanSOperatorType::said),new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *dummy4 = new BanStatementList({new BanDataList({A}),new BanStatementList({new BanDataList({B}),new BanDataList({Na}),new BanSOperator(BanSOperatorType::believes)}) ,new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *dummy5 = new BanStatementList({new BanDataList({A}),new BanDataList({Na,Nb,new BanDOperator(BanDOperatorType::concates)}),new BanSOperator(BanSOperatorType::believes)});


    BanStatementList *dummy6 = new BanStatementList({new BanDataList({B}),new BanDataList({Na}),new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *dummy7 = new BanStatementList({new BanDataList({A}),dummy6,new BanSOperator(BanSOperatorType::believes)});
    BanStatementList *dummy8 = new BanStatementList({new BanDataList({A}),new BanDataList({Na}),new BanSOperator(BanSOperatorType::believes)});

    assum.append(a1);
    assum.append(a2);
    assum.append(a3);
    assum.append(a4);
    assum.append(a5);
    assum.append(a6);
    assum.append(a7);
    assum.append(a8);
    assum.append(a9);

    step.append(s1);
    step.append(s2);
    step.append(s3);
    step.append(s4);
    step.append(dummy8);

    goals.append(g1);
    goals.append(dummy1);

    goals.append(dummy7);
    goals.append(dummy5);
    goals.append(dummy2);
 goals.append(dummy4);
 goals.append(dummy3);

 //  goals.append(g1);

  //goals.append(g2);
  //  goals.append(g3);
   //  goals.append(g4);


    //        BanDAtom *a = new BanDAtom(banAtomtype::Principal, "A");
    //        BanDAtom *b = new BanDAtom(banAtomtype::Principal, "B");
    //        BanDAtom *n = new BanDAtom(banAtomtype::Nonce, "Na");
    //        BanDAtom *nb = new BanDAtom(banAtomtype::Nonce, "Nb");
    //        BanDAtom *k = new BanDAtom(banAtomtype::SymKey, "Kab");
    //        QList<BanStatementList*> preList;
    //        //BanRuleId::F1 : P believes fresh (X) -> P believes fresh (X,Y)
    //        BanStatementList *a1 = new BanStatementList({new BanDataList({b}), new BanDataList({n,new BanDOperator(BanDOperatorType::FreshData)}),new BanSOperator(BanSOperatorType::believes)});
    //        BanStatementList *s1 = new BanStatementList({new BanDataList({b}), new BanDataList({a,new BanDOperator(BanDOperatorType::FreshData)}), new BanSOperator(BanSOperatorType::believes)});
    //        BanStatementList *g1 = new BanStatementList({new BanDataList({b}), new BanDataList({a,n,new BanDOperator(BanDOperatorType::concates),new BanDOperator(BanDOperatorType::FreshData)}), new BanSOperator(BanSOperatorType::believes)});
    //       // BanStatementList *g2 = new BanStatementList({new BanDataList({b}), new BanDataList({n}), new BanSOperator(BanSOperatorType::sees)});
    //        assum.append(a1);
    //        step.append(s1);
    //        //QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool", s1->getString());
    //        goals.append(g1);
    //       // goals.append(g2);
    //        BanStatementList *a2 = new BanStatementList({new BanDataList({b}), new BanDataList({k}),new BanSOperator(BanSOperatorType::sees)});
    //        BanStatementList *s2 = new BanStatementList({new BanDataList({b}), new BanDataList({nb,new BanDOperator(BanDOperatorType::FreshData)}), new BanSOperator(BanSOperatorType::believes)});
    //        BanStatementList *g2 = new BanStatementList({new BanDataList({b}), new BanDataList({nb,k,new BanDOperator(BanDOperatorType::Encryption)}),new BanSOperator(BanSOperatorType::sees)});
    //        assum.append(a2);
    //        step.append(s2);
    //        //QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool", s1->getString());
    //        goals.append(g2);
    //       // goals.append(g2);

    //        BanStatementList *a3 = new BanStatementList({new BanDataList({b}), new BanDataList({k}),new BanSOperator(BanSOperatorType::sees)});
    //        BanStatementList *s3 = new BanStatementList({new BanDataList({b}), new BanDataList({a,n,new BanDOperator(BanDOperatorType::concates)}), new BanSOperator(BanSOperatorType::sees)});
    //        BanStatementList *g3 = new BanStatementList({new BanDataList({b}), new BanDataList({k}),new BanSOperator(BanSOperatorType::sees)});
    // assum.append(a3);
    //   step.append(s3);
    //QMessageBox::information(nullptr, "Cryptographic-Protocol Development & Verification Tool", s1->getString());
    //   goals.append(g3);
    // goals.append(g2);

    LPT::LayeredProvingTree *lpt = new LPT::LayeredProvingTree(LPT::ImplementedLogics::BAN, assum, step, goals,comment);

    lpt->printInfo();

    std::cout << "known statement = " << lpt->getKnownTrueStatementCount() << std::endl;
    lpt->verifyProtocol();
    std::cout << "known statement = " << lpt->getKnownTrueStatementCount() << std::endl;
    return lpt;}

