#include "politsake.h"

#include "memopzkconverter.h"

#include <QDesktopServices>
#include <QIcon>
#include <QMessageBox>

PolitSake::PolitSake()
{
    resize(800, 600);
    setMinimumSize(800, 600);

    setWindowIcon(QIcon(":/pics/favicon.ico"));
    setWindowTitle("PolitSake");

    lineEditURL = new QLineEdit{this};
    lineEditURL->setText(prisonersListURL);
    lineEditURL->setGeometry(10, 10, 350, 22);

    pushButtonLoad = new QPushButton{this};
    pushButtonLoad->setText("Load");
    pushButtonLoad->setGeometry(380, 10, 100, 22);
    pushButtonLoad->setEnabled(false);

    pushButtonBrowse = new QPushButton{this};
    pushButtonBrowse->setText("Browse");
    pushButtonBrowse->setGeometry(380, 40, 100, 22);

    pushButtonQR = new QPushButton{this};
    pushButtonQR->setText("QR Code");
    pushButtonQR->setGeometry(380, 70, 100, 22);
    pushButtonQR->setEnabled(false);

    pushButtonWriteLetter = new QPushButton{this};
    pushButtonWriteLetter->setIcon(QIcon(":/pics/couvert.bmp"));
    pushButtonWriteLetter->setGeometry(10, 60, 32, 32);

    lcdNumberLettersCount = new QLCDNumber{this};
    lcdNumberLettersCount->display(0);
    lcdNumberLettersCount->setDigitCount(5);
    lcdNumberLettersCount->setGeometry(50, 60, 80, 32);

    penitentiaryDatabase = new PenitentiaryDatabase{this};
    penitentiaryDatabase->setVisible(false);

    prisonersListView = new PrisonersListView{this, prisonersToAmenities};
    prisonersListView->setGeometry(10, 100, 350, 470);

    frameViewWeb = new QFrame{this};
    frameViewWeb->setFrameStyle(QFrame::Panel | QFrame::Raised);
    frameViewWeb->setGeometry(380, 100, 410, 470);

    labelCurrentPrisonerIntro = new QLabel{this};
    labelCurrentPrisonerIntro->setText("Prisoner:");
    labelCurrentPrisonerIntro->setGeometry(10, 570, 60, 22);

    labelCurrentPrisonerText = new QLabel{this};
    labelCurrentPrisonerText->setGeometry(70, 570, 720, 22);

    connect(pushButtonBrowse, SIGNAL(clicked()), this, SLOT(browseURL()));

    connect(pushButtonWriteLetter, SIGNAL(clicked()), this, SLOT(writeLetter()));

    connect(prisonersListView, SIGNAL(clicked(QModelIndex)), this,
            SLOT(updateCurrentPrisoner(QModelIndex)));
    connect(prisonersListView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this,
            SLOT(updateLettersCount()));
}

void PolitSake::browseURL()
{
    if (!currentPrisonerIndex.isValid()) {
        QMessageBox::information(this, "Information", "Prisoner not clicked");
        return;
    }

    QString currentPrisoner = currentPrisonerIndex.data().toString();
    QDesktopServices::openUrl(QUrl{MemoPZKConverter::convertToURL(currentPrisoner)});
}

void PolitSake::updateCurrentPrisoner(QModelIndex modelIndex)
{
    currentPrisonerIndex = modelIndex;

    labelCurrentPrisonerText->setText(currentPrisonerIndex.data().toString());
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

    QMessageBox::information(this, "Letter Address", QString{"%1\n%2\n%3\n%4 %5"}.arg(currentPrisoner,
                                                                                      amenityName,
                                                                                      amenityAddress.location,
                                                                                      amenityAddress.zip,
                                                                                      amenityAddress.state));
}
