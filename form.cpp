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
    ui->log->hide();

    showFullScreen();

    setSize(ui->log, ui->stateLable);


    QObject::connect(&cp, SIGNAL(sendState(QString)), this, SLOT(showState(QString)));

    QObject::connect(&LOG_WRITER, SIGNAL(setText(QString)), ui->log, SLOT(append(QString)));
    QObject::connect(&LOG_WRITER, SIGNAL(setColor(QColor)), ui->log, SLOT(setTextColor(QColor)));
}

Form::~Form()
{
    delete ui;
}

void Form::showState(const QString state)
{
    QString train("                   ooOOOO\n                oo          _____\n              _I__n_n__||_|| ________\n          >(_________|_7_|-|______|\n            /o  ()()   ()()   o      oo  oo");
    if (ui->log->isHidden())
    {
        ui->trainLable->show();
        ui->log->show();
        moveAboutCenter(ui->trainLable, ui->log->width()/2+45, 40);
        moveAboutCenter(ui->stateLable, ui->log->width()/2, 25);
    }
    ui->stateLable->setText(state);
    ui->trainLable->setAlignment(Qt::AlignLeft);
    ui->trainLable->setText(train);
}

void Form::moveAboutCenter(QLabel *lable, int x, int y)
{
    QPoint point = QDesktopWidget().availableGeometry().center();
    point.setY(point.y() + y - lable->height()/2);
    point.setX(point.x() + x - lable->width()/2);
    lable->move(point);
}

void Form::setSize(QTextEdit *textLog, QLabel *lable)
{
    QRect screen = QApplication::desktop()->screenGeometry();
    screen.setWidth(screen.width()/2);
    textLog->setGeometry(screen);
    lable->setGeometry(screen);
}


