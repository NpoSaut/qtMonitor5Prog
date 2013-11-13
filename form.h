#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include "Fudp/canprog.h"
#include "PropStore/simplefilepropstore.h"

using namespace Fudp;

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT
    
public:
    explicit Form(SimpleFilePropStore *pStore, QWidget *parent = 0);
    ~Form();
    
private:
    Ui::Form *ui;
    CanProg cp;
    void moveAboutCenter(QWidget *frame, int x, int y);
    void setSize(QWidget *frame);

signals:
    void setSerialNumber(qint32 serialNumber);

private slots:
    void showState(const QString state);
    void inputSerialNumber();
    void on_blockSerialNumberOk_pressed();
};

#endif // FORM_H
