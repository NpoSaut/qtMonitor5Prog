#include "form.h"
#include "ui_form.h"
#include <QDesktopWidget>

Form::Form(const CanProgWorker *canProgWorker, Parser *parser, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),
    sn(0)
{
    ui->setupUi(this);

    hideElements();

    showFullScreen();

    getProps();
    if (sn!=0)
        showProps(true);

    // Подключение кнопок CAN-клавиатуры
    QObject::connect (&parser->consoleKey1, SIGNAL(keyPressed (ConsoleKey::ConsKey key)), this, SLOT(on_CanKeyboard_KeyPressed(ConsoleKey::ConsKey key)));
    QObject::connect (&parser->consoleKey2, SIGNAL(keyPressed (ConsoleKey::ConsKey key)), this, SLOT(on_CanKeyboard_KeyPressed(ConsoleKey::ConsKey key)));

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
    if (state == tr("Прошивка повреждена"))
        showProps(true);
    else
        showProps(false);
}

void Form::initLables()
{
    QString train("                   ooOOOO\n                oo          _____\n              _I__n_n__||_||_ _______\n          >(_________|_7_|-|______|\n            /o  ()()   ()()   o      oo  oo");
    ui->trainLable->setAlignment(Qt::AlignLeft);
    ui->trainLable->setText(train);
    moveAboutCenter(ui->trainLable, 0, 0);
    ui->trainLable->show();

    moveAboutCenter(ui->stateLable, 0, 30);
    ui->stateLable->show();
//        ui->log->show();    
}

void Form::inputSerialNumber()
{
    ui->lableBlockSerialNumber->show();

    ui->editblockSerialNumber->show();
    ui->editblockSerialNumber->setFocus();

    showProps(false);
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
    moveAboutCenter(ui->lableBlockSerialNumber, 0, -50);
    moveAboutCenter(ui->editblockSerialNumber, 0, 0);
    moveAboutCenter(ui->keyboard, 0, 100);
    ui->editblockSerialNumber->setMaxLength(6);
    if (show)
        ui->keyboard->show();
    else
        ui->keyboard->hide();
}

void Form::showProps(bool show)
{
    if (show)
    {
        moveAboutCenter(ui->serialNumberLable,QApplication::desktop()->geometry().width()/2 - ui->serialNumberLable->geometry().width()/2,QApplication::desktop()->geometry().height()/2);
        moveAboutCenter(ui->versionLable,- QApplication::desktop()->geometry().width()/2 + ui->versionLable->geometry().width()/2,QApplication::desktop()->geometry().height()/2);

        ui->serialNumberLable->setText(QString(tr("Сериный номер: %1")).arg(sn));
        ui->versionLable->setText(QString(tr("Версия: %1.%2")).arg(vers).arg(subvers));
        ui->serialNumberLable->show();
        ui->versionLable->show();
    }
    else
    {
        ui->serialNumberLable->hide();
        ui->versionLable->hide();
    }
}

void Form::getProps()
{
    QFile file("C:/MonMSUL/prop.txt");
    SimpleFilePropStore props(file);
    props.get(1,vers);
    props.get(2,subvers);
    props.get(131, sn);
}

void Form::on_blockSerialNumberOk_pressed()
{
    if(ui->editblockSerialNumber->text()!="")
    {
        ui->editblockSerialNumber->hide();
        ui->lableBlockSerialNumber->hide();
        showKeyboard(false);
        sn = (qint32)ui->editblockSerialNumber->text().toInt();
        emit setSerialNumber(sn);
    }
}

void Form::on_CanKeyboard_KeyPressed(ConsoleKey::ConsKey key)
{
    switch (key) {
    case ConsoleKey::F1: on_oneButton_clicked (); break;
    case ConsoleKey::F2: on_twoButton_clicked (); break;
    case ConsoleKey::F3: on_threeButton_clicked (); break;
    case ConsoleKey::F4: on_fourButton_clicked (); break;
    case ConsoleKey::F5: on_fiveButton_clicked (); break;
    case ConsoleKey::F6: on_sixButton_clicked (); break;
    case ConsoleKey::F7: on_sevenButton_clicked (); break;
    case ConsoleKey::F8: on_eightButton_clicked (); break;
    case ConsoleKey::F9: on_nineButton_clicked (); break;
    case ConsoleKey::F0: break;
    case ConsoleKey::BKSP: on_backSpace_clicked (); break;
    case ConsoleKey::ENTER: on_blockSerialNumberOk_pressed (); break;
    default:
        break;
    }
}

void Form::hideElements()
{
    ui->trainLable->hide();
    ui->stateLable->hide();
    ui->editblockSerialNumber->hide();
    ui->lableBlockSerialNumber->hide();
    showKeyboard(false);
    showProps(false);
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

void Form::on_nullButton_clicked()
{
    ui->editblockSerialNumber->setText(ui->editblockSerialNumber->text() + "0");
    ui->editblockSerialNumber->setFocus();
}

void Form::on_backSpace_clicked()
{
    ui->editblockSerialNumber->backspace();;
    ui->editblockSerialNumber->setFocus();
}

void Form::on_blockSerialNumberOk_clicked()
{
    on_blockSerialNumberOk_pressed();
}

void Form::on_clear_clicked()
{
    ui->editblockSerialNumber->clear();
    ui->editblockSerialNumber->setFocus();
}

void Form::on_yearOk_clicked()
{
    ui->yearOk->setShortcut(Qt::Key_Return);
    ui->yearOk->hide();
    ui->lableBlockSerialNumber->setText(tr("Введите месяц"));
    on_clear_clicked();
}

void Form::on_monthOk_clicked()
{
    ui->monthOk->setShortcut(Qt::Key_Return);
    ui->monthOk->hide();
    ui->lableBlockSerialNumber->setText(tr("Введите серийный номер"));
    on_clear_clicked();
}
