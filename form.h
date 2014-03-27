#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>

#include "canprogworker.h"
#include "qtBlokLib/parser.h"

using namespace Fudp;

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT
    
public:
    explicit Form(const CanProgWorker *canProgWorker, Parser *parser, QWidget *parent = 0);
    ~Form();
    
private:
    Ui::Form *ui;
    void moveAboutCenter(QWidget *frame, int x, int y);
    void setSize(QWidget *frame);
    void showKeyboard(bool show);

signals:
    void setSerialNumber(qint32 serialNumber);

public slots:
    void showState(const QString state);

private slots:
    void onModeChanged (bool progMode) { if (progMode) initLables(); else hideElements(); }
    void inputSerialNumber();
    void on_blockSerialNumberOk_pressed();

    void on_CanKeyboard_KeyPressed(ConsoleKey::ConsKey key);

    void on_oneButton_clicked();

    void on_twoButton_clicked();

    void on_threeButton_clicked();

    void on_fourButton_clicked();

    void on_fiveButton_clicked();

    void on_sixButton_clicked();

    void on_sevenButton_clicked();

    void on_eightButton_clicked();

    void on_nineButton_clicked();

    void on_backSpace_clicked();

    void on_blockSerialNumberOk_clicked();

private:
    void initLables();
    void hideElements();

};

#endif // FORM_H
