#include "constloaderstore.h"

ConstLoaderStore::ConstLoaderStore(qint32 loaderVersion, qint32 protocolVersion, qint32 compatibleProtocolVersion)
{
    map[192] = 1;
    map[193] = loaderVersion;
    map[194] = 1;
    map[195] = protocolVersion;
    map[196] = compatibleProtocolVersion;
}

QList<quint8> ConstLoaderStore::keys() const
{
    return map.keys();
}

QVector<QPair<quint8, qint32> > ConstLoaderStore::data() const
{
    QVector< QPair<quint8, qint32> > data;
    foreach (quint8 key, map.keys())
        data.append( QPair<quint8, qint32> (key, map[key]) );
    return data;
}

bool ConstLoaderStore::get(quint8 key, qint32 &value) const
{
    if ( map.find(key) == map.end() )
        return false;
    else
        value = map[key];
    return true;
}

bool ConstLoaderStore::set(quint8 key, qint32 value)
{
    return false;
}

bool ConstLoaderStore::del(quint8 key)
{
    return false;
}

bool ConstLoaderStore::sync()
{
    return true;
}

bool ConstLoaderStore::discard()
{
    return true;
}

