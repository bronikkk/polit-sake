#ifndef POLITSAKE_H
#define POLITSAKE_H

#include <QAbstractTableModel>
#include <QBoxLayout>
#include <QLCDNumber>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QPushButton>
#include <QStatusBar>
#include <QTabWidget>

#ifdef QT_WEBENGINEWIDGETS_LIB
#include <QtWebEngineWidgets/QWebEngineView>
#endif

#include "penitentiarydatabase.h"
#include "prisonerslistview.h"

class PolitSake : public QTabWidget
{
    Q_OBJECT

public:
    PolitSake();

    static constexpr auto prisonersListURL = "https://api.airtable.com/v0/appaVayeFV2RS6Ihv/PZK";
    static constexpr auto airTableToken =
        "patkxJoU705lxPpKs.befb0b2bed420853d25fb2908a2496b19df2f44c109d7b36c23cf0236fcf1c83";

private:
    QLCDNumber *lcdNumberLettersCount;
    QLabel *labelFacilityAddress, *labelWriteLetter;
    QLineEdit *lineEditURL;
    QPushButton *pushButtonBrowse, *pushButtonCopy, *pushButtonSearch, *pushButtonWriteLetter;

#ifdef QT_WEBENGINEWIDGETS_LIB
    QWebEngineView *webEngineView;
#endif

    QWidget *personsTab, *prisonsTab;

    // TODO: Fix this
    QModelIndex currentPrisonerIndex;
    QMap<QString, QString> prisonersToFacilities;

    PenitentiaryDatabase *penitentiaryDatabase;
    PrisonersListView *prisonersListView;

private slots:
    void browsePrisoner();

    void copyPrisonerInformation();

    void searchPrisoner();

    void updateCurrentFacility(QModelIndex modelIndex);

    void updateCurrentPrisoner(QModelIndex modelIndex);

    void updateLettersCount();

    void writeLetter();
};

#endif // POLITSAKE_H
