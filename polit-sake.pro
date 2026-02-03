# You can make your code get rid of WebEngine.
DEFINES += USE_WEBENGINE

QT += core gui network widgets

contains(DEFINES, USE_WEBENGINE): QT += webenginewidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    memopzkconverter.cpp \
    penitentiarydatabase.cpp \
    politsake.cpp \
    prisonerslistmodel.cpp

HEADERS += \
    memopzkconverter.h \
    penitentiarydatabase.h \
    politsake.h \
    prisonerslistmodel.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    politsakeressource.qrc

TRANSLATIONS += \
    translations/polit_sake_be_BY.ts \
    translations/polit_sake_en_001.ts \
    translations/polit_sake_ru_BY.ts \
    translations/polit_sake_ru_RU.ts \
    translations/polit_sake_ru_UA.ts \
    translations/polit_sake_uk_UA.ts
