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

#include "prisonerslistview.h"

class PolitSake : public QWidget
{
    Q_OBJECT

public:
    PolitSake();

    static constexpr auto prisonersListURL = "https://api.airtable.com/v0/appaVayeFV2RS6Ihv/PZK";
    static constexpr auto airTableToken =
        "patkxJoU705lxPpKs.6e7432e3ace8f9f5b0416f32c11031c49c7d928145418458ed7670b784eca4ba";

private:
    QFrame *frameViewWeb;
    QLabel *labelCurrentPrisonerIntro, *labelCurrentPrisonerText;
    QLCDNumber *lcdNumberLettersCount;
    QLineEdit *lineEditURL;
    QPushButton *pushButtonBrowse, *pushButtonLoad, *pushButtonQR, *pushButtonWriteLetter;
    PrisonersListView *listView;

    // TODO: Fix this
    QMap<QString, QString> lettersAddresses;

    QModelIndex currentPrisonerIndex;

private slots:
    void browseURL();

    void updateCurrentPrisoner(QModelIndex modelIndex);

    void updateLettersCount();

    void writeLetter();
};

#endif // POLITSAKE_H
