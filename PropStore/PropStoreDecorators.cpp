#include "PropStoreDecorators.h"

PropStoreDecorator::PropStoreDecorator(PropStore *store)
    : store (store)
{ }


PropStoreWithKeyRange::PropStoreWithKeyRange(PropStore *store, quint8 from, quint8 to)
    : PropStoreDecorator (store), from (from), to (to)
{ }

bool PropStoreWithKeyRange::discard()
{
    bool success = true;

    success = PropStoreDecorator::discard () && success;
    foreach (quint8 key, PropStoreDecorator::keys ())
        if ( !isInRange (key) )
            success = PropStoreDecorator::del (key) && success;

    return success;
}
