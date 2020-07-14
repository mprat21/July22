QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BAN/bandatac.cpp \
    BAN/bandatalist.cpp \
    BAN/bandatom.cpp \
    BAN/bandcomponent.cpp \
    BAN/bandoperator.cpp \
    BAN/banexception.cpp \
    BAN/banlogicimpl.cpp \
    BAN/banpostulates.cpp \
    BAN/banscomponent.cpp \
    BAN/bansoperator.cpp \
    BAN/banstatementlist.cpp \
    GNY/gnyatom.cpp \
    GNY/gnycomponent.cpp \
    GNY/gnydata.cpp \
    GNY/gnylogic.cpp \
    GNY/gnypostulate.cpp \
    GNY/gnystatement.cpp \
    LPT/Layeredprovingtree.cpp \
    LPT/Logic.cpp \
    LPT/Postulate.cpp \
    LPT/Statement.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    BAN/banEnumTypes.h \
    BAN/bancontainer.h \
    BAN/bandatac.h \
    BAN/bandatalist.h \
    BAN/bandatom.h \
    BAN/bandcomponent.h \
    BAN/bandoperator.h \
    BAN/banexception.h \
    BAN/banlogicimpl.h \
    BAN/banpostulates.h \
    BAN/banscomponent.h \
    BAN/bansoperator.h \
    BAN/banstatementlist.h \
    GNY/gnyatom.h \
    GNY/gnycomponent.h \
    GNY/gnycontainer.h \
    GNY/gnydata.h \
    GNY/gnydefinitions.h \
    GNY/gnylogic.h \
    GNY/gnypostulate.h \
    GNY/gnystatement.h \
    LPT/LPTContainers.h \
    LPT/LPTDefinitions.h \
    LPT/LPTExceptions.h \
    LPT/LPTList.h \
    LPT/LPTQueue.h \
    LPT/LPTStack.h \
    LPT/Layeredprovingtree.h \
    LPT/Logic.h \
    LPT/Postulate.h \
    LPT/Statement.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
