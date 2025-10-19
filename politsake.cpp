#include "politsake.h"

#include "memopzkconverter.h"

#include <QClipboard>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QIcon>
#include <QMessageBox>

PolitSake::PolitSake()
{
    resize(800, 570);
    setMinimumSize(800, 570);

    setWindowIcon(QIcon(":/pics/favicon.ico"));
    setWindowTitle("PolitSake");

    personsTab = new QWidget{this};

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

    pushButtonWriteLetter = new QPushButton{personsTab};
    pushButtonWriteLetter->setIcon(QIcon(":/pics/couvert.bmp"));
    pushButtonWriteLetter->setGeometry(10, 60, 32, 32);

    lcdNumberLettersCount = new QLCDNumber{personsTab};
    lcdNumberLettersCount->display(0);
    lcdNumberLettersCount->setDigitCount(5);
    lcdNumberLettersCount->setGeometry(50, 60, 80, 32);

    penitentiaryDatabase = new PenitentiaryDatabase{personsTab};
    penitentiaryDatabase->setVisible(false);

    prisonersListView = new PrisonersListView{personsTab, prisonersToAmenities};
    prisonersListView->setGeometry(10, 100, 350, 440);

    frameViewWeb = new QFrame{personsTab};
    frameViewWeb->setFrameStyle(QFrame::Panel | QFrame::Raised);
    frameViewWeb->setGeometry(380, 100, 410, 440);

    addTab(personsTab, "Persons");

    prisonsTab = new QWidget{this};

    addTab(prisonsTab, "Prisons");

    connect(pushButtonBrowse, SIGNAL(clicked()), this, SLOT(browsePrisoner()));

    connect(pushButtonCopy, SIGNAL(clicked()), this, SLOT(copyPrisonerInformation()));

    connect(pushButtonSearch, SIGNAL(clicked()), this, SLOT(searchPrisoner()));

    connect(pushButtonWriteLetter, SIGNAL(clicked()), this, SLOT(writeLetter()));

    connect(prisonersListView, SIGNAL(clicked(QModelIndex)), this,
            SLOT(updateCurrentPrisoner(QModelIndex)));
    connect(prisonersListView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this,
            SLOT(updateLettersCount()));
}

void PolitSake::browsePrisoner()
{
    if (!currentPrisonerIndex.isValid()) {
        QMessageBox::information(this, "Information", "Prisoner not clicked");
        return;
    }

    QString currentPrisoner = currentPrisonerIndex.data().toString();
    QDesktopServices::openUrl(QUrl{MemoPZKConverter::convertToURL(currentPrisoner)});
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
    QDesktopServices::openUrl(QUrl{MemoPZKConverter::convertToSearchURL(currentPrisoner)});
}

void PolitSake::updateCurrentPrisoner(QModelIndex modelIndex)
{
    currentPrisonerIndex = modelIndex;
}

void PolitSake::updateLettersCount()
{
    lcdNumberLettersCount->display(prisonersListView->getSize());
}

void PolitSake::writeLetter()
{
    if (!currentPrisonerIndex.isValid()) {
        QMessageBox::information(this, "Information", "Prisoner not clicked");
        return;
    }

    prisonersListView->model()->setData(currentPrisonerIndex, Qt::Checked, Qt::CheckStateRole);

    QString currentPrisoner = currentPrisonerIndex.data().toString();
    auto amenityNameIndex = prisonersToAmenities.find(currentPrisoner);

    QString amenityName;
    if (amenityNameIndex != prisonersToAmenities.end()) {
        amenityName = *amenityNameIndex;
    }

    PenitentiaryDatabase::Address amenityAddress = penitentiaryDatabase->getAddressForPenitentiary(
                                                       amenityName);

    QMessageBox::information(this, "Letter Address", QString{"%1\n%2\n%3\n%4 %5"}.arg(currentPrisoner)
                             .arg(amenityName)
                             .arg(amenityAddress.location)
                             .arg(amenityAddress.zip)
                             .arg(amenityAddress.state));
}
