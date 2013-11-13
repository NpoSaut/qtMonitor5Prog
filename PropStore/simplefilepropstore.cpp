#include "simplefilepropstore.h"

#include <QTextStream>

#include <QDebug>

#include <QStringList>

SimpleFilePropStore::SimpleFilePropStore(QFile &file)
    : file (file), map ()
{
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream fileIn(&file);
    while (!fileIn.atEnd())
    {
        QStringList line(fileIn.readLine().split(" "));

        if(line.at(0) != "")
            map[QString(line.at(0)).toInt()] = QString(line.at(1)).toInt();
    }

    file.close();
}

QList<quint8> SimpleFilePropStore::keys() const
{
    return map.keys();
}

QVector<QPair<quint8, qint32> > SimpleFilePropStore::data() const
{
    QVector< QPair<quint8, qint32> > data;
    foreach (quint8 key, map.keys())
        data.append( QPair<quint8, qint32> (key, map[key]) );
    return data;
}

bool SimpleFilePropStore::get(quint8 key, qint32 &value) const
{
    if ( map.find(key) == map.end() )
        return false;
    else
        value = map[key];
    return true;
}

bool SimpleFilePropStore::set(quint8 key, qint32 value)
{
    map[key] = value;
    bool success = (map[key] == value); // :)

    return sync() & success;
}

bool SimpleFilePropStore::del(quint8 key)
{
    map.erase(map.find (key));
    return sync();
}

bool SimpleFilePropStore::sync()
{
    if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream out(&file);
        foreach (quint8 k, map.keys())
            out << k << " " << map[k] << endl;

        file.close();
        return true;
    }
    else
    {
        return false;
    }
}
