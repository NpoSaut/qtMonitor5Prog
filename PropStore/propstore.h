#ifndef PROPSTORE_H
#define PROPSTORE_H

#include <QList>
#include <QVector>
#include <QPair>

// Интерфейс хранилища словаря свойств
//  Позволяет хранить пары "ключ (1 байт) - значение (4 байта)"
class PropStore
{
public:
    // Выводит список всех хранимых ключей
    virtual QList<quint8> keys() const = 0;

    // Возаращает все хранимые данные
    virtual QVector< QPair<quint8, qint32> > data() const = 0;

    // Считать свойство по ключу
    //  Значения возвращается по ссылке, успешность по return
    virtual bool get (quint8 key, qint32 &value) const = 0;

    // Записать свойство по ключу
    //  В случае отсутсвия, свойство добавляется. Успешность по return
    virtual bool set (quint8 key, qint32 value) = 0;

    // Удаляет свойство
    //  Возвращает успешность операции
    virtual bool del (quint8 key) = 0;
};

#endif // PROPSTORE_H
