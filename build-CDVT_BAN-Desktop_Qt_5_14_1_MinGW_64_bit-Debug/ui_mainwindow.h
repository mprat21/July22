/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *commandLayout;
    QPushButton *loadProtocol;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QComboBox *comboBox;
    QRadioButton *radioButton;
    QComboBox *comboBox_2;
    QPushButton *startVerification;
    QPushButton *pushButton_2;
    QTextBrowser *comments;
    QTreeView *treeView;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1463, 964);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        commandLayout = new QVBoxLayout();
        commandLayout->setSpacing(20);
        commandLayout->setObjectName(QString::fromUtf8("commandLayout"));
        commandLayout->setSizeConstraint(QLayout::SetFixedSize);
        commandLayout->setContentsMargins(20, 20, 20, -1);
        loadProtocol = new QPushButton(centralwidget);
        loadProtocol->setObjectName(QString::fromUtf8("loadProtocol"));

        commandLayout->addWidget(loadProtocol);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(200, 50));
        label->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        verticalLayout_2->addWidget(label);

        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        verticalLayout_2->addWidget(comboBox);


        commandLayout->addLayout(verticalLayout_2);

        radioButton = new QRadioButton(centralwidget);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        commandLayout->addWidget(radioButton);

        comboBox_2 = new QComboBox(centralwidget);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        commandLayout->addWidget(comboBox_2);

        startVerification = new QPushButton(centralwidget);
        startVerification->setObjectName(QString::fromUtf8("startVerification"));

        commandLayout->addWidget(startVerification);

        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        commandLayout->addWidget(pushButton_2);

        comments = new QTextBrowser(centralwidget);
        comments->setObjectName(QString::fromUtf8("comments"));
        comments->setMaximumSize(QSize(300, 16777215));

        commandLayout->addWidget(comments);

        commandLayout->setStretch(0, 1);
        commandLayout->setStretch(1, 100);

        horizontalLayout->addLayout(commandLayout);

        treeView = new QTreeView(centralwidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        horizontalLayout->addWidget(treeView);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1463, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(startVerification, SIGNAL(pressed()), MainWindow, SLOT(startVerification()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        loadProtocol->setText(QCoreApplication::translate("MainWindow", "Load Protocol", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Select Search Strategy:", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Depth First Search", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Inverse DFS", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Breath First Search", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Inverse BFS", nullptr));

        radioButton->setText(QCoreApplication::translate("MainWindow", "Exhaustive Verification", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("MainWindow", "GNY Logic", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("MainWindow", "BAN Logic", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("MainWindow", "CDVT Logic", nullptr));
        comboBox_2->setItemText(3, QCoreApplication::translate("MainWindow", "CDVT + Attack Detection", nullptr));

        startVerification->setText(QCoreApplication::translate("MainWindow", "Start Verification", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Reset Verification", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
