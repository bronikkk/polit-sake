#include <QApplication>
#include <QTranslator>

#include "politsake.h"

int main(int argc, char *argv[])
{
    QApplication app{argc, argv};

    QTranslator translator;
    if (translator.load(":/translations/polit_sake_en_001.qm")) {
        app.installTranslator(&translator);
    }

    PolitSake politSake;
    politSake.show();

    return app.exec();
}
