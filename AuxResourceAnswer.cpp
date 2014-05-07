#include "AuxResourceAnswer.h"

#include "qtBlokLib/parsers/aux_resource.h"

AuxResourceAnswer::AuxResourceAnswer(Can *can, Parser *parser, int auxNumber, int auxDescriptor, int version, int subversion, int checksum, QObject *parent) :
    QObject(parent),
    can (can),
    number (auxNumber),
    descriptor (auxDescriptor),
    version (version),
    subversion (subversion),
    checksum (checksum)
{
    this->connect (&parser->sysDiagnostics, SIGNAL(versionRequested(SysDiagnostics::AuxModule)), SLOT(catchRequest(SysDiagnostics::AuxModule)));
}

void AuxResourceAnswer::sendVersion()
{
    AuxResourceVersion message (AuxResource::Descriptor(descriptor), this);
    message.setVersion (version);
    message.setSubversion (subversion);
    message.setChecksum (checksum);
    can->transmitMessage (message.encode ());
}

void AuxResourceAnswer::catchRequest(SysDiagnostics::AuxModule module)
{
    if ( int (module) == number )
        sendVersion ();
}
