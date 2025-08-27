#include <QApplication>

#include "politsake.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PolitSake politSake;
    politSake.show();
    return app.exec();
}
