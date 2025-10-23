QT += core gui network widgets webenginewidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    memopzkconverter.cpp \
    penitentiarydatabase.cpp \
    politsake.cpp \
    prisonerslistmodel.cpp \
    prisonerslistview.cpp

HEADERS += \
    memopzkconverter.h \
    penitentiarydatabase.h \
    politsake.h \
    prisonerslistmodel.h \
    prisonerslistview.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    politsakeressource.qrc
