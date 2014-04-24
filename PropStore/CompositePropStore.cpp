#include "CompositePropStore.h"

QList<quint8> CompositePropStore::keys() const
{
    QList<quint8> result;

    foreach (PropStore *store, stores)
        result += store->keys();

    return result;
}

QVector<QPair<quint8, qint32> > CompositePropStore::data() const
{
    QVector< QPair<quint8, qint32> > result;

    foreach (PropStore *store, stores)
        result += store->data ();

    return result;
}

bool CompositePropStore::get(quint8 key, qint32 &value) const
{
    foreach (PropStore *store, stores)
    {
        if ( store->get (key, value) )
            return true;
    }
    return false;
}

bool CompositePropStore::set(quint8 key, qint32 value)
{
    foreach (PropStore *store, stores)
        if ( store->keys ().indexOf (key) != -1 )
            return store->set (key, value);

    foreach (PropStore *store, stores)
        if ( store->set (key, value) )
            return true;

    return false;
}

bool CompositePropStore::del(quint8 key)
{
    bool success = false;

    foreach (PropStore *store, stores)
        success = store->del (key) || success;

    return success;
}

bool CompositePropStore::sync()
{
    bool success = false;

    foreach (PropStore *store, stores)
        success = store->sync () && success;

    return success;
}

bool CompositePropStore::discard()
{
    bool success = false;

    foreach (PropStore *store, stores)
        success = store->discard () && success;

    return success;
}
