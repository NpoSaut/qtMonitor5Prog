#ifndef COMPOSITEPROPSTORE_H
#define COMPOSITEPROPSTORE_H

#include "PropStore.h"

// Список хранилищ
class CompositePropStore : public PropStore
{
public:
    CompositePropStore (QList<PropStore *> &stores)
        : stores (stores)
    { }

    // Выводит список всех хранимых ключей во всех хранилищах
    virtual QList<quint8> keys() const;

    // Возаращает все хранимые данные во всех хранилищах
    virtual QVector< QPair<quint8, qint32> > data() const;

    // Ищет свойство среди всех хранилищ и возвращает его
    //  Значения возвращается по ссылке, успешность по return
    virtual bool get (quint8 key, qint32 &value) const;

    // Записывает свойство по ключу
    //  Будет произведен поиск этого свойства среди всех хранилищь:
    //   - если оно будет найдено, то будет произведена попытка перезаписи в этом хранилище, результат по return;
    //   - если оно нигде не существовало, то будет создано в первом хранилище,
    //     в котором это получится сделать. Если нигде не получится: то false по return;
    virtual bool set (quint8 key, qint32 value);

    // Удаляет свойство из всех хранилищ
    //  Возвращает true, если хотя бы из одного хранилища произведено удаление
    virtual bool del (quint8 key);

    // Синхронизирует данные в кэше и файле для всех хранилищ (записывает в файл)
    virtual bool sync ();

    // Обновляет локальную версию данных из файла для всех хранилищ (не записанные изменения теряются)
    virtual bool discard ();

protected:
    QList<PropStore *> stores;
};

#endif // COMPOSITEPROPSTORE_H
