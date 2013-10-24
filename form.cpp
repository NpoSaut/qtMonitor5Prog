#include "form.h"
#include "ui_form.h"
#include <windows.h>
#include <QDesktopWidget>

Form::Form(SimpleFilePropStore *pStore, QWidget *parent) :
    cp(pStore),
    log(),
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    showFullScreen();

    setSize(ui->log);
    moveAboutCenter(ui->label, ui->log->width()/2, 0);

    QObject::connect(&cp, SIGNAL(sendState(QString)), this, SLOT(showState(QString)));

    QObject::connect(&log, SIGNAL(getText(QString)), ui->log, SLOT(append(QString)));
}

Form::~Form()
{
    delete ui;
}

void Form::showState(const QString state)
{
    moveAboutCenter(ui->stateLable, ui->log->width()/2, 100);
    ui->stateLable->setText(state);
}

void Form::moveAboutCenter(QLabel *lable, int x, int y)
{
    QPoint point = QDesktopWidget().availableGeometry().center();
    point.setY(point.y() + y - lable->height()/2);
    point.setX(point.x() + x - lable->width()/2);
    lable->move(point);
}

void Form::setSize(QTextEdit *log)
{
    QRect screen = QApplication::desktop()->screenGeometry();
    screen.setWidth(screen.width()/2);
    log->setGeometry(screen);
}


