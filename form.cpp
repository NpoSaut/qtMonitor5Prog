#include "form.h"
#include "ui_form.h"
#include <windows.h>
#include <QDesktopWidget>

Form::Form(const CanProgWorker *canProgWorker, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    ui->blockSerialNumberOk->setShortcut(Qt::Key_Return);
    ui->blockSerialNumberOk->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));

    hideElements();

    showFullScreen();

    QObject::connect(canProgWorker, SIGNAL(stateChanged(QString)), this, SLOT(showState(QString)));
    QObject::connect(canProgWorker, SIGNAL(serialNumberMissed()), this, SLOT(inputSerialNumber()));
    QObject::connect(canProgWorker, SIGNAL(inProgModeChanged(bool)), this, SLOT(onModeChanged(bool)));
    QObject::connect(this, SIGNAL(setSerialNumber(qint32)), canProgWorker, SLOT(storeSerialNumber(qint32)));
}

Form::~Form()
{
    delete ui;
}

void Form::showState(const QString state)
{
    ui->stateLable->setText(state);
}

void Form::initLables()
{
    QString train("                   ooOOOO\n                oo          _____\n              _I__n_n__||_||_ _______\n          >(_________|_7_|-|______|\n            /o  ()()   ()()   o      oo  oo");

    ui->trainLable->show();
    ui->stateLable->show();
//        ui->log->show();
    moveAboutCenter(ui->trainLable, 0, 0);
    moveAboutCenter(ui->stateLable, 0, 30);
    ui->trainLable->setAlignment(Qt::AlignLeft);
    ui->trainLable->setText(train);
}

void Form::inputSerialNumber()
{
    moveAboutCenter(ui->lableBlockSerialNumber, 0, -75);
    ui->lableBlockSerialNumber->show();

    moveAboutCenter(ui->editblockSerialNumber, 0, 0);
    ui->editblockSerialNumber->show();
    ui->editblockSerialNumber->setFocus();

    moveAboutCenter(ui->blockSerialNumberOk, 0, 50);
    ui->blockSerialNumberOk->show();
}

void Form::moveAboutCenter(QWidget *frame, int x, int y)
{
    QPoint point = QDesktopWidget().availableGeometry().center();
    point.setY(point.y() + y - frame->height()/2);
    point.setX(point.x() + x - frame->width()/2);
    frame->move(point);
}

void Form::setSize(QWidget *frame)
{
    QRect screen = QApplication::desktop()->screenGeometry();
    screen.setWidth(screen.width()/2);
    frame->setGeometry(screen);
}

void Form::on_blockSerialNumberOk_pressed()
{
    if(ui->editblockSerialNumber->text()!="")
    {
        ui->blockSerialNumberOk->hide();
        ui->editblockSerialNumber->hide();
        ui->lableBlockSerialNumber->hide();

        emit setSerialNumber((qint32)ui->editblockSerialNumber->text().toInt());
    }
}

void Form::hideElements()
{
    ui->trainLable->hide();
    ui->stateLable->hide();
    ui->blockSerialNumberOk->hide();
    ui->editblockSerialNumber->hide();
    ui->lableBlockSerialNumber->hide();
}
