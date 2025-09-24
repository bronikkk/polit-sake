#ifndef POLITSAKE_H
#define POLITSAKE_H

#include <QAbstractTableModel>
#include <QFrame>
#include <QLCDNumber>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QPushButton>
#include <QWidget>

#include "penitentiarydatabase.h"
#include "prisonerslistview.h"

class PolitSake : public QWidget
{
    Q_OBJECT

public:
    PolitSake();

    static constexpr auto prisonersListURL = "https://api.airtable.com/v0/appaVayeFV2RS6Ihv/PZK";
    static constexpr auto airTableToken =
        "patkxJoU705lxPpKs.befb0b2bed420853d25fb2908a2496b19df2f44c109d7b36c23cf0236fcf1c83";

private:
    QFrame *frameViewWeb;
    QLabel *labelCurrentPrisonerIntro, *labelCurrentPrisonerText;
    QLCDNumber *lcdNumberLettersCount;
    QLineEdit *lineEditURL;
    QPushButton *pushButtonBrowse, *pushButtonSearch, *pushButtonQR, *pushButtonWriteLetter;

    // TODO: Fix this
    QMap<QString, QString> prisonersToAmenities;
    QModelIndex currentPrisonerIndex;

    PenitentiaryDatabase *penitentiaryDatabase;
    PrisonersListView *prisonersListView;

private slots:
    void browsePrisoner();

    void searchPrisoner();

    void updateCurrentPrisoner(QModelIndex modelIndex);

    void updateLettersCount();

    void writeLetter();
};

#endif // POLITSAKE_H
