#ifndef PROPSTOREDECORATORS_H
#define PROPSTOREDECORATORS_H

#include "PropStore.h"

class PropStoreDecorator : public PropStore
{
public:
    PropStoreDecorator(PropStore *store);

public:
    // Выводит список всех хранимых ключей
    virtual QList<quint8> keys() const { return store->keys (); }

    // Возаращает все хранимые данные
    virtual QVector< QPair<quint8, qint32> > data() const { return store->data (); }

    // Считать свойство по ключу
    //  Значения возвращается по ссылке, успешность по return
    virtual bool get (quint8 key, qint32 &value) const { return store->get (key, value); }

    // Записать свойство по ключу
    //  В случае отсутсвия, свойство добавляется. Успешность по return
    virtual bool set (quint8 key, qint32 value) { return store->set (key, value); }

    // Удаляет свойство
    //  Возвращает успешность операции
    virtual bool del (quint8 key) { return store->del (key); }

    // Записывает данные из кеша в хранилище
    virtual bool sync () { return store->sync (); }

    // Читает данные из хранилища в кеш, затирая его
    virtual bool discard () { return store->discard (); }

private:
    PropStore * const store;
};

// Выделяет из store только те записи, которые находятся в диапазоне [from, to].
class PropStoreWithKeyRange : public PropStoreDecorator
{
public:
    PropStoreWithKeyRange(PropStore *store, quint8 from, quint8 to);

    // Выводит список всех хранимых ключей
    virtual QList<quint8> keys() const;

    // Возаращает все хранимые данные
    virtual QVector< QPair<quint8, qint32> > data() const;

    // Считать свойство по ключу
    //  Значения возвращается по ссылке, успешность по return
    virtual bool get (quint8 key, qint32 &value) const;

    // Записать свойство по ключу, если ключь лежит в диапазоне [from, to]
    //  В случае отсутсвия, свойство добавляется. Успешность по return
    virtual bool set (quint8 key, qint32 value);

    // Удаляет свойство
    //  Возвращает успешность операции
    virtual bool del (quint8 key);

    // Записывает данные из кеша в хранилище
    virtual bool sync () { return PropStoreDecorator::sync (); }

    // Читает данные из хранилища в кеш, затирая его
    virtual bool discard () { return PropStoreDecorator::discard(); }

private:
    bool isInRange (quint8 key) const { return from <= key && key <= to; }
    quint8 from, to;
};

#endif // PROPSTOREDECORATORS_H
