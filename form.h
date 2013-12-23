#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>

#include "canprogworker.h"

using namespace Fudp;

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT
    
public:
    explicit Form(const CanProgWorker *canProgWorker, QWidget *parent = 0);
    ~Form();
    
private:
    Ui::Form *ui;
    void moveAboutCenter(QWidget *frame, int x, int y);
    void setSize(QWidget *frame);

signals:
    void setSerialNumber(qint32 serialNumber);

public slots:
    void showState(const QString state);

private slots:
    void onModeChanged (bool progMode) { if (progMode) initLables(); else hideElements(); }
    void inputSerialNumber();
    void on_blockSerialNumberOk_pressed();

private:
    void initLables();
    void hideElements();

};

#endif // FORM_H
