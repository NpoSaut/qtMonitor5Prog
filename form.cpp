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

    showFullScreen();

    moveAboutCenter(ui->label, 0, 0);

    QObject::connect(&cp, SIGNAL(sendState(QString)), this, SLOT(showState(QString)));
}

Form::~Form()
{
    delete ui;
}

void Form::showState(const QString state)
{
    moveAboutCenter(ui->stateLable, 0, 100);
    ui->stateLable->setText(state);
}

void Form::moveAboutCenter(QLabel *lable, int x, int y)
{
    QPoint point = QDesktopWidget().availableGeometry().center();
    point.setY(point.y() + y - lable->height()/2);
    point.setX(point.x() + x - lable->width()/2);
    lable->move(point);
}

