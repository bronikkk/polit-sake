#include "politsake.h"

#include <QDesktopServices>
#include <QIcon>
#include <QMessageBox>

PolitSake::PolitSake()
{
    resize(800, 600);
    setMinimumSize(800, 600);

    setWindowIcon(QIcon(":/pics/favicon.ico"));
    setWindowTitle("PolitSake");

    lineEditURL = new QLineEdit(this);
    lineEditURL->setText(prisonersListURL);
    lineEditURL->setGeometry(10, 10, 350, 22);

    pushButtonLoad = new QPushButton(this);
    pushButtonLoad->setText("Load");
    pushButtonLoad->setGeometry(380, 10, 100, 22);
    pushButtonLoad->setEnabled(false);

    pushButtonBrowse = new QPushButton(this);
    pushButtonBrowse->setText("Browse");
    pushButtonBrowse->setGeometry(380, 40, 100, 22);

    pushButtonQR = new QPushButton(this);
    pushButtonQR->setText("QR Code");
    pushButtonQR->setGeometry(380, 70, 100, 22);
    pushButtonQR->setEnabled(false);

    pushButtonWriteLetter = new QPushButton(this);
    pushButtonWriteLetter->setIcon(QIcon(":/pics/couvert.bmp"));
    pushButtonWriteLetter->setGeometry(10, 60, 32, 32);

    lcdNumberLettersCount = new QLCDNumber(this);
    lcdNumberLettersCount->display(0);
    lcdNumberLettersCount->setDigitCount(5);
    lcdNumberLettersCount->setGeometry(50, 60, 80, 32);

    listView = new PrisonersListView(this, lettersAddresses);
    listView->setGeometry(10, 100, 350, 470);

    frameViewWeb = new QFrame(this);
    frameViewWeb->setFrameStyle(QFrame::Panel | QFrame::Raised);
    frameViewWeb->setGeometry(380, 100, 410, 470);

    labelCurrentPrisonerIntro = new QLabel(this);
    labelCurrentPrisonerIntro->setText("Prisoner:");
    labelCurrentPrisonerIntro->setGeometry(10, 570, 60, 22);

    labelCurrentPrisonerText = new QLabel(this);
    labelCurrentPrisonerText->setGeometry(70, 570, 720, 22);

    QObject::connect(pushButtonBrowse, SIGNAL(clicked()), this, SLOT(browseURL()));

    QObject::connect(pushButtonWriteLetter, SIGNAL(clicked()), this, SLOT(writeLetter()));

    QObject::connect(listView, SIGNAL(clicked(QModelIndex)), this,
                     SLOT(updateCurrentPrisoner(QModelIndex)));
    QObject::connect(listView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this,
                     SLOT(updateLettersCount()));
}

void PolitSake::browseURL()
{
    QDesktopServices::openUrl(QUrl{lineEditURL->text()});
}

void PolitSake::updateCurrentPrisoner(QModelIndex modelIndex)
{
    currentPrisonerIndex = modelIndex;

    labelCurrentPrisonerText->setText(currentPrisonerIndex.data().toString());
}

void PolitSake::updateLettersCount()
{
    lcdNumberLettersCount->display(listView->getSize());
}

void PolitSake::writeLetter()
{
    if (!currentPrisonerIndex.isValid()) {
        QMessageBox::information(this, "Information", "Prisoner not clicked");
        return;
    }

    listView->model()->setData(currentPrisonerIndex, Qt::Checked, Qt::CheckStateRole);

    QString currentPrisoner = currentPrisonerIndex.data().toString();
    auto currentLetterAddressIndex = lettersAddresses.find(currentPrisoner);

    QString currentLetterAddress;
    if (currentLetterAddressIndex != lettersAddresses.end()) {
        currentLetterAddress = *currentLetterAddressIndex;
    }

    QMessageBox::information(this, "Letter Address", currentLetterAddress);
}
