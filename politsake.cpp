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
    setWindowTitle(tr("PolitSake"));

    personsTab = new QWidget{this};
    prisonsTab = new QWidget{this};

    lineEditURL = new QLineEdit{personsTab};
    lineEditURL->setText(prisonersListURL);
    lineEditURL->setGeometry(10, 10, 350, 22);

    pushButtonWriteLetter = new QPushButton{personsTab};
    pushButtonWriteLetter->setIcon(QIcon(":/pics/couvert.bmp"));
    pushButtonWriteLetter->setGeometry(10, 35, 32, 32);
    pushButtonWriteLetter->setShortcut(QKeySequence{Qt::CTRL | Qt::Key_W});

    labelWriteLetter = new QLabel{personsTab};
    labelWriteLetter->setText(tr("⌘W"));
    labelWriteLetter->setGeometry(42, 35, 32, 32);

    lcdNumberLettersCount = new QLCDNumber{personsTab};
    lcdNumberLettersCount->display(0);
    lcdNumberLettersCount->setDigitCount(5);
    lcdNumberLettersCount->setGeometry(80, 35, 80, 32);

    pushButtonBrowse = new QPushButton{personsTab};
    pushButtonBrowse->setText(tr("Browse"));
    pushButtonBrowse->setGeometry(170, 40, 60, 22);

    pushButtonSearch = new QPushButton{personsTab};
    pushButtonSearch->setText(tr("Search"));
    pushButtonSearch->setGeometry(235, 40, 60, 22);

    pushButtonCopy = new QPushButton{personsTab};
    pushButtonCopy->setText(tr("Copy"));
    pushButtonCopy->setGeometry(300, 40, 60, 22);

    prisonersListView = new QListView{personsTab};
    prisonersListView->setEnabled(false);
    prisonersListView->setGeometry(10, 75, 350, 635);

    prisonersListModel = nullptr;
    QMetaObject::invokeMethod(this, &PolitSake::generatePrisonersListModel, Qt::QueuedConnection);

#ifdef QT_WEBENGINEWIDGETS_LIB
    webEngineView = new QWebEngineView{personsTab};
    webEngineView->setGeometry(380, 0, 644, 715);

    webEngineView->load(MemoPZKConverter::getLoadingPage());
    webEngineView->show();
#endif

    setTabOrder(lineEditURL, pushButtonWriteLetter);
    setTabOrder(pushButtonWriteLetter, prisonersListView);
    setTabOrder(prisonersListView, pushButtonBrowse);
    setTabOrder(pushButtonBrowse, pushButtonSearch);
    setTabOrder(pushButtonSearch, pushButtonCopy);

    addTab(personsTab, tr("&Persons"));

    labelFacilityAddress = new QLabel{prisonsTab};
    labelFacilityAddress->setGeometry(10, 10, 780, 80);

    penitentiaryDatabase = new PenitentiaryDatabase{prisonsTab};
    penitentiaryDatabase->setGeometry(10, 100, 1010, 610);

    addTab(prisonsTab, tr("&Facilities"));

    connect(pushButtonBrowse, SIGNAL(clicked()), this, SLOT(browsePrisoner()));

    connect(pushButtonCopy, SIGNAL(clicked()), this, SLOT(copyPrisonerInformation()));

    connect(pushButtonSearch, SIGNAL(clicked()), this, SLOT(searchPrisoner()));

    connect(pushButtonWriteLetter, SIGNAL(clicked()), this, SLOT(writeLetter()));

    connect(penitentiaryDatabase->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(updateCurrentFacility(QModelIndex)));
}

void PolitSake::browsePrisoner()
{
    if (!currentPrisonerIndex.isValid()) {
        QMessageBox::information(this, tr("Information"), tr("Prisoner not clicked"));
        return;
    }

    QString currentPrisoner = currentPrisonerIndex.data().toString();
    QDesktopServices::openUrl(MemoPZKConverter::convertToURL(currentPrisoner));
}

void PolitSake::copyPrisonerInformation()
{
    if (!currentPrisonerIndex.isValid()) {
        QMessageBox::information(this, tr("Information"), tr("Prisoner not clicked"));
        return;
    }

    QString currentPrisoner = currentPrisonerIndex.data().toString();
    QGuiApplication::clipboard()->setText(currentPrisoner);
}

void PolitSake::searchPrisoner()
{
    if (!currentPrisonerIndex.isValid()) {
        QMessageBox::information(this, tr("Information"), tr("Prisoner not clicked"));
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

    QString facilityAddressText = facilityName;
    if (!facilityAddress.zip.isEmpty()) {
        facilityAddressText += QString{"\n%1\n%2 %3"}.arg(facilityAddress.location)
                               .arg(facilityAddress.zip)
                               .arg(facilityAddress.state);
    }

    labelFacilityAddress->setText(facilityAddressText);
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
    lcdNumberLettersCount->display(prisonersListModel->getSize());
}

void PolitSake::writeLetter()
{
    if (!currentPrisonerIndex.isValid()) {
        QMessageBox::information(this, tr("Information"), tr("Prisoner not clicked"));
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

    QString letterAddressText = QString{"%1\n%2"}.arg(currentPrisoner).arg(facilityName);
    if (!facilityAddress.zip.isEmpty()) {
        letterAddressText += QString{"\n%1\n%2 %3"}.arg(facilityAddress.location).arg(
                                 facilityAddress.zip).arg(facilityAddress.state);
    }

    QMessageBox::information(this, tr("Information"), letterAddressText);


    prisonersListView->setFocus();
}

void PolitSake::generatePrisonersListModel()
{
    prisonersListModel = new PrisonersListModel{personsTab, prisonersListView, prisonersToFacilities};

    connect(prisonersListView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this,
            SLOT(updateLettersCount()));
    connect(prisonersListView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this,
            SLOT(updateCurrentPrisoner(QModelIndex)));
}
