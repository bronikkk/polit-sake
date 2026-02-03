#ifndef MEMOPZKCONVERTER_H
#define MEMOPZKCONVERTER_H

#include <QString>

class MemoPZKConverter
{
public:
    static QString convertToSearchURL(QString prisonerName);

    static QString convertToURL(QString prisonerName);

    static QString getLoadingPage();
};

#endif // MEMOPZKCONVERTER_H
