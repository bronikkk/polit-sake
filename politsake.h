#ifndef POLITSAKE_H
#define POLITSAKE_H

#include <QAbstractTableModel>
#include <QFrame>
#include <QLCDNumber>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include "prisonerslistview.h"

class PolitSake : public QWidget
{
    Q_OBJECT

public:
    PolitSake();

private:
    static constexpr auto prisonersListURL =
        "https://docs.google.com/spreadsheets/d/12YNoH7w86lw2NfCbMZaa9t0TiAaECTyIKbEvn2MEwp8/edit";

    QFrame *frameViewWeb;
    QLabel *labelCurrentPrisonerIntro, *labelCurrentPrisonerText;
    QLCDNumber *lcdNumberLettersCount;
    QLineEdit *lineEditURL;
    QPushButton *pushButtonBrowse, *pushButtonLoad, *pushButtonQR, *pushButtonWriteLetter;
    PrisonersListView *listView;

    QModelIndex currentPrisonerIndex;

private slots:
    void browseURL();

    void updateCurrentPrisoner(QModelIndex modelIndex);

    void updateLettersCount();

    void writeLetter();
};

#endif // POLITSAKE_H
