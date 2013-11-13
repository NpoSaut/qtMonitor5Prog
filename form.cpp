#include "form.h"
#include "ui_form.h"
#include <windows.h>
#include <QDesktopWidget>

Form::Form(SimpleFilePropStore *pStore, QWidget *parent) :
    cp(pStore),
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    ui->trainLable->hide();

    ui->blockSerialNumberOk->hide();
    ui->editblockSerialNumber->hide();
    ui->lableBlockSerialNumber->hide();

    showFullScreen();

    QObject::connect(&cp, SIGNAL(sendState(QString)), this, SLOT(showState(QString)));
    QObject::connect(&cp, SIGNAL(noSerialNumber()), this, SLOT(inputSerialNumber()));
    QObject::connect(this, SIGNAL(setSerialNumber(qint32)), &cp, SLOT(inputBlockSerialNumber(qint32)));
}

Form::~Form()
{
    delete ui;
}

void Form::showState(const QString state)
{
    QString train("                   ooOOOO\n                oo          _____\n              _I__n_n__||_||_ _______\n          >(_________|_7_|-|______|\n            /o  ()()   ()()   o      oo  oo");
    if (ui->trainLable->isHidden())
    {
        ui->trainLable->show();
        moveAboutCenter(ui->trainLable, 0, 0);
        moveAboutCenter(ui->stateLable, 0, 0);
    }
    ui->stateLable->setText(state);
    ui->trainLable->setAlignment(Qt::AlignLeft);
    ui->trainLable->setText(train);
}

void Form::inputSerialNumber()
{
    moveAboutCenter(ui->lableBlockSerialNumber, 0, -25);
    ui->lableBlockSerialNumber->show();

    moveAboutCenter(ui->editblockSerialNumber, 0, 0);
    ui->editblockSerialNumber->show();

    moveAboutCenter(ui->blockSerialNumberOk, 0, 25);
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
    ui->blockSerialNumberOk->hide();
    ui->editblockSerialNumber->hide();
    ui->lableBlockSerialNumber->hide();

    emit setSerialNumber((qint32)ui->editblockSerialNumber->text().toInt());
}
