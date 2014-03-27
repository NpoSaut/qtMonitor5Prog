#include "form.h"
#include "ui_form.h"
#include <QDesktopWidget>

Form::Form(const CanProgWorker *canProgWorker, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    ui->blockSerialNumberOk->setShortcut(Qt::Key_Return);
    ui->blockSerialNumberOk->setStyleSheet(QString::fromUtf8("background-color: rgb(240, 240, 240);"));

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
    moveAboutCenter(ui->lableBlockSerialNumber, 0, -50);
    ui->lableBlockSerialNumber->show();

    moveAboutCenter(ui->editblockSerialNumber, 0, 0);
    ui->editblockSerialNumber->show();
    ui->editblockSerialNumber->setFocus();

    moveAboutCenter(ui->keyboard, 0, 100);
    showKeyboard(true);
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

void Form::showKeyboard(bool show)
{
    if (show)
        ui->keyboard->show();
    else
        ui->keyboard->hide();
}

void Form::on_blockSerialNumberOk_pressed()
{
    if(ui->editblockSerialNumber->text()!="")
    {
        ui->editblockSerialNumber->hide();
        ui->lableBlockSerialNumber->hide();
        showKeyboard(false);

        emit setSerialNumber((qint32)ui->editblockSerialNumber->text().toInt());
    }
}

void Form::hideElements()
{
    ui->trainLable->hide();
    ui->stateLable->hide();
    ui->editblockSerialNumber->hide();
    ui->lableBlockSerialNumber->hide();
    showKeyboard(false);
}

void Form::on_oneButton_clicked()
{
    ui->editblockSerialNumber->setText(ui->editblockSerialNumber->text() + "1");
    ui->editblockSerialNumber->setFocus();
}

void Form::on_twoButton_clicked()
{
    ui->editblockSerialNumber->setText(ui->editblockSerialNumber->text() + "2");
    ui->editblockSerialNumber->setFocus();
}

void Form::on_threeButton_clicked()
{
    ui->editblockSerialNumber->setText(ui->editblockSerialNumber->text() + "3");
    ui->editblockSerialNumber->setFocus();
}

void Form::on_fourButton_clicked()
{
    ui->editblockSerialNumber->setText(ui->editblockSerialNumber->text() + "4");
    ui->editblockSerialNumber->setFocus();
}

void Form::on_fiveButton_clicked()
{
    ui->editblockSerialNumber->setText(ui->editblockSerialNumber->text() + "5");
    ui->editblockSerialNumber->setFocus();
}

void Form::on_sixButton_clicked()
{
    ui->editblockSerialNumber->setText(ui->editblockSerialNumber->text() + "6");
    ui->editblockSerialNumber->setFocus();
}

void Form::on_sevenButton_clicked()
{
    ui->editblockSerialNumber->setText(ui->editblockSerialNumber->text() + "7");
    ui->editblockSerialNumber->setFocus();
}

void Form::on_eightButton_clicked()
{
    ui->editblockSerialNumber->setText(ui->editblockSerialNumber->text() + "8");
    ui->editblockSerialNumber->setFocus();
}

void Form::on_nineButton_clicked()
{
    ui->editblockSerialNumber->setText(ui->editblockSerialNumber->text() + "9");
    ui->editblockSerialNumber->setFocus();
}

void Form::on_backSpace_clicked()
{
    QString str = ui->editblockSerialNumber->text();
    qint32 length = ui->editblockSerialNumber->text().length();
    ui->editblockSerialNumber->setText(str.remove(length-1, 1));
    ui->editblockSerialNumber->setFocus();
}

void Form::on_blockSerialNumberOk_clicked()
{
    on_blockSerialNumberOk_pressed();
}
