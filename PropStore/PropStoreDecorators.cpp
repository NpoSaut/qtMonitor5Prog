#include "PropStoreDecorators.h"

PropStoreDecorator::PropStoreDecorator(PropStore *store)
    : store (store)
{ }


PropStoreWithKeyRange::PropStoreWithKeyRange(PropStore *store, quint8 from, quint8 to)
    : PropStoreDecorator (store), from (from), to (to)
{ }


QList<quint8> PropStoreWithKeyRange::keys() const
{
    QList<quint8> result;
    foreach (quint8 key, PropStoreDecorator::keys ()) {
        if (isInRange(key))
            result.append(key);
    }
    return result;
}

QVector<QPair<quint8, qint32> > PropStoreWithKeyRange::data() const
{
    QVector<QPair<quint8, qint32> > result;
    foreach (quint8 key, PropStoreDecorator::keys ()) {
        if (isInRange(key))
        {
            QPair<quint8, qint32> pair;
            if (PropStoreDecorator::get(pair.first, pair.second))
                result.append(pair);
        }
    }
    return result;
}

bool PropStoreWithKeyRange::get(quint8 key, qint32 &value) const
{
    if (isInRange(key))
        return PropStoreDecorator::get (key, value);
    else
        return false;
}

bool PropStoreWithKeyRange::set(quint8 key, qint32 value)
{
    if (isInRange(key))
        return PropStoreDecorator::set(key, value);
    else
        return false;
}

bool PropStoreWithKeyRange::del(quint8 key)
{
    if (isInRange(key))
        return PropStoreDecorator::del (key);
    else
        return false;
}
