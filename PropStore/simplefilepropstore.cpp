#include "simplefilepropstore.h"

#ifdef MONITOR_5
#include <io.h>
#endif

#include <QTextStream>
#include <QStringList>

SimpleFilePropStore::SimpleFilePropStore(QFile &file)
    : file (file), map ()
{
    discard ();
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

    return success;
}

bool SimpleFilePropStore::del(quint8 key)
{
    map.erase(map.find(key));
    return true;
}

bool SimpleFilePropStore::sync()
{
    SimpleFilePropStore storeInFile (file);
    if ( map == storeInFile.map )
    {
        return true;
    }
    else
    {
        if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream out(&file);
            foreach (quint8 k, map.keys())
                out << k << " " << map[k] << endl;
            file.flush();
#ifdef MONITOR_5
            _commit(file.handle());
#endif
            file.close();
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool SimpleFilePropStore::discard()
{
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream fileIn(&file);
    while (!fileIn.atEnd())
    {
        QStringList line(fileIn.readLine().split(" "));

        if(line.at(0) != "" && line.size() == 2)
            map[QString(line.at(0)).toInt()] = QString(line.at(1)).toInt();
    }

    file.close();
}
