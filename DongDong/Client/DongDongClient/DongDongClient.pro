QT       += core gui network sql core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addgroup.cpp \
    cancelsign.cpp \
    choosedfriend.cpp \
    choosefriend.cpp \
    conversation.cpp \
    friendmessage.cpp \
    friendrequestwidget.cpp \
    friendrequestwindow.cpp \
    friendwidget.cpp \
    groupwidget.cpp \
    loginandregister.cpp \
    main.cpp \
    mainwindow.cpp \
    mychatwidget.cpp \
    mymessage.cpp \
    queryuserwidget.cpp \
    searchwidget.cpp \
    setinfomation.cpp \
    setremarkwidget.cpp \
    user.cpp

HEADERS += \
    addgroup.h \
    cancelsign.h \
    choosedfriend.h \
    choosefriend.h \
    config.h \
    conversation.h \
    friendmessage.h \
    friendrequestwidget.h \
    friendrequestwindow.h \
    friendwidget.h \
    groupwidget.h \
    loginandregister.h \
    mainwindow.h \
    mychatwidget.h \
    mymessage.h \
    queryuserwidget.h \
    searchwidget.h \
    setinfomation.h \
    setremarkwidget.h \
    user.h

FORMS += \
    addgroup.ui \
    choosedfriend.ui \
    choosefriend.ui \
    conversation.ui \
    friendmessage.ui \
    friendrequestwidget.ui \
    friendrequestwindow.ui \
    friendwidget.ui \
    groupwidget.ui \
    loginandregister.ui \
    mainwindow.ui \
    mychatwidget.ui \
    mymessage.ui \
    queryuserwidget.ui \
    setinfomation.ui \
    setremarkwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
