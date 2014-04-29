#ifndef AUXRESOURCEANSWER_H
#define AUXRESOURCEANSWER_H

#include <QObject>

#include "qtCanLib/can.h"
#include "qtBlokLib/parser.h"

class AuxResourceAnswer : public QObject
{
    Q_OBJECT
public:
    explicit AuxResourceAnswer(Can *can, Parser *parser, int auxNumber, int auxDescriptor, int version, int subversion, int checksum, QObject *parent = 0);
    
    int getAuxNumber () const { return number; }
    int getAuxDescriptor () const { return descriptor; }

    void setAuxNumber (int n) { number = n; }
    void setAuxDescriptor (int d) { descriptor = d; }

public slots:
    void sendVersion ();

private slots:
    void catchRequest (SysDiagnostics::AuxModule module);
    
private:
    Can *can;
    int number;
    int descriptor;
    int version;
    int subversion;
    int checksum;
};

#endif // AUXRESOURCEANSWER_H
