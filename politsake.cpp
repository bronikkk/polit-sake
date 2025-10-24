#include "politsake.h"

#include "memopzkconverter.h"

#include <QClipboard>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QIcon>
#include <QMessageBox>

PolitSake::PolitSake()
{
    resize(1024, 740);
    setMinimumSize(1024, 740);

    setWindowIcon(QIcon(":/pics/favicon.ico"));
    setWindowTitle("PolitSake");

    personsTab = new QWidget{this};
    prisonsTab = new QWidget{this};

    lineEditURL = new QLineEdit{personsTab};
    lineEditURL->setText(prisonersListURL);
    lineEditURL->setGeometry(10, 10, 350, 22);

    pushButtonBrowse = new QPushButton{personsTab};
    pushButtonBrowse->setText("Browse");
    pushButtonBrowse->setGeometry(380, 10, 100, 22);

    pushButtonSearch = new QPushButton{personsTab};
    pushButtonSearch->setText("Search");
    pushButtonSearch->setGeometry(380, 40, 100, 22);

    pushButtonCopy = new QPushButton{personsTab};
    pushButtonCopy->setText("Copy");
    pushButtonCopy->setGeometry(380, 70, 100, 22);

    labelWriteLetter = new QLabel{personsTab};
    labelWriteLetter->setText("Ctrl + w");
    labelWriteLetter->setGeometry(10, 35, 350, 22);

    pushButtonWriteLetter = new QPushButton{personsTab};
    pushButtonWriteLetter->setIcon(QIcon(":/pics/couvert.bmp"));
    pushButtonWriteLetter->setGeometry(10, 60, 32, 32);
    pushButtonWriteLetter->setShortcut(QKeySequence{Qt::CTRL | Qt::Key_W});

    lcdNumberLettersCount = new QLCDNumber{personsTab};
    lcdNumberLettersCount->display(0);
    lcdNumberLettersCount->setDigitCount(5);
    lcdNumberLettersCount->setGeometry(50, 60, 80, 32);

    prisonersListView = new PrisonersListView{personsTab, prisonersToFacilities};
    prisonersListView->setGeometry(10, 100, 350, 630);

#ifdef QT_WEBENGINEWIDGETS_LIB
    webEngineView = new QWebEngineView{personsTab};
    webEngineView->setGeometry(380, 100, 644, 630);
#endif

    setTabOrder(lineEditURL, pushButtonWriteLetter);
    setTabOrder(pushButtonWriteLetter, prisonersListView);
    setTabOrder(prisonersListView, pushButtonBrowse);
    setTabOrder(pushButtonBrowse, pushButtonSearch);
    setTabOrder(pushButtonSearch, pushButtonCopy);

    addTab(personsTab, "&Persons");

    labelFacilityAddress = new QLabel{prisonsTab};
    labelFacilityAddress->setGeometry(10, 10, 780, 80);

    penitentiaryDatabase = new PenitentiaryDatabase{prisonsTab};
    penitentiaryDatabase->setGeometry(10, 100, 780, 630);

    addTab(prisonsTab, "&Facilities");

    connect(pushButtonBrowse, SIGNAL(clicked()), this, SLOT(browsePrisoner()));

    connect(pushButtonCopy, SIGNAL(clicked()), this, SLOT(copyPrisonerInformation()));

    connect(pushButtonSearch, SIGNAL(clicked()), this, SLOT(searchPrisoner()));

    connect(pushButtonWriteLetter, SIGNAL(clicked()), this, SLOT(writeLetter()));

    connect(prisonersListView->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex)), this,
            SLOT(updateLettersCount()));
    connect(prisonersListView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this,
            SLOT(updateCurrentPrisoner(QModelIndex)));

    connect(penitentiaryDatabase->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(updateCurrentFacility(QModelIndex)));
}

void PolitSake::browsePrisoner()
{
    if (!currentPrisonerIndex.isValid()) {
        QMessageBox::information(this, "Information", "Prisoner not clicked");
        return;
    }

    QString currentPrisoner = currentPrisonerIndex.data().toString();
    QDesktopServices::openUrl(MemoPZKConverter::convertToURL(currentPrisoner));
}

void PolitSake::copyPrisonerInformation()
{
    if (!currentPrisonerIndex.isValid()) {
        QMessageBox::information(this, "Information", "Prisoner not clicked");
        return;
    }

    QString currentPrisoner = currentPrisonerIndex.data().toString();
    QGuiApplication::clipboard()->setText(currentPrisoner);
}

void PolitSake::searchPrisoner()
{
    if (!currentPrisonerIndex.isValid()) {
        QMessageBox::information(this, "Information", "Prisoner not clicked");
        return;
    }

    QString currentPrisoner = currentPrisonerIndex.data().toString();
    QDesktopServices::openUrl(MemoPZKConverter::convertToSearchURL(currentPrisoner));
}

void PolitSake::updateCurrentFacility(QModelIndex modelIndex)
{
    QString facilityName = modelIndex.data().toString();

    PenitentiaryDatabase::Address facilityAddress = penitentiaryDatabase->getAddressForPenitentiary(
                                                        facilityName);

    labelFacilityAddress->setText(QString{"%1\n%2\n%3 %4"}.arg(facilityName)
                                  .arg(facilityAddress.location)
                                  .arg(facilityAddress.zip)
                                  .arg(facilityAddress.state));
}

void PolitSake::updateCurrentPrisoner(QModelIndex modelIndex)
{
    currentPrisonerIndex = modelIndex;

#ifdef QT_WEBENGINEWIDGETS_LIB
    QString currentPrisoner = currentPrisonerIndex.data().toString();
    webEngineView->load(MemoPZKConverter::convertToURL(currentPrisoner));
    webEngineView->show();
#endif
}

void PolitSake::updateLettersCount()
{
    lcdNumberLettersCount->display(prisonersListView->getModelSize());
}

void PolitSake::writeLetter()
{
    if (!currentPrisonerIndex.isValid()) {
        QMessageBox::information(this, "Information", "Prisoner not clicked");
        return;
    }

    prisonersListView->model()->setData(currentPrisonerIndex, Qt::Checked, Qt::CheckStateRole);

    QString currentPrisoner = currentPrisonerIndex.data().toString();
    auto facilityNameIndex = prisonersToFacilities.find(currentPrisoner);

    QString facilityName;
    if (facilityNameIndex != prisonersToFacilities.end()) {
        facilityName = *facilityNameIndex;
    }

    PenitentiaryDatabase::Address facilityAddress = penitentiaryDatabase->getAddressForPenitentiary(
                                                        facilityName);

    QMessageBox::information(this, "Letter Address", QString{"%1\n%2\n%3\n%4 %5"}.arg(currentPrisoner)
                             .arg(facilityName)
                             .arg(facilityAddress.location)
                             .arg(facilityAddress.zip)
                             .arg(facilityAddress.state));

    prisonersListView->setFocus();
}
