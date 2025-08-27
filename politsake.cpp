#include "politsake.h"

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
    lineEditURL->setGeometry(10, 30, 350, 22);

    pushButtonOpen = new QPushButton(this);
    pushButtonOpen->setText("Open");
    pushButtonOpen->setGeometry(380, 30, 100, 22);

    pushButtonWriteLetter = new QPushButton(this);
    pushButtonWriteLetter->setIcon(QIcon(":/pics/couvert.bmp"));
    pushButtonWriteLetter->setGeometry(10, 60, 32, 32);

    lcdNumberLettersCount = new QLCDNumber(this);
    lcdNumberLettersCount->display(0);
    lcdNumberLettersCount->setDigitCount(5);
    lcdNumberLettersCount->setGeometry(50, 60, 80, 32);

    listView = new PrisonersListView(this);
    listView->setGeometry(10, 100, 350, 470);

    labelCurrentPrisonerIntro = new QLabel(this);
    labelCurrentPrisonerIntro->setText("Prisoner:");
    labelCurrentPrisonerIntro->setGeometry(10, 570, 60, 22);

    labelCurrentPrisonerText = new QLabel(this);
    labelCurrentPrisonerText->setGeometry(70, 570, 350, 22);

    QObject::connect(pushButtonWriteLetter, SIGNAL(clicked()), this, SLOT(writeLetter()));

    QObject::connect(listView, SIGNAL(clicked(QModelIndex)), this,
                     SLOT(updateCurrentPrisoner(QModelIndex)));
    QObject::connect(listView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this,
                     SLOT(updateLettersCount()));
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

    QString letterAddress = currentPrisonerIndex.data().toString();
    QMessageBox::information(this, "Letter Address", letterAddress);
}
