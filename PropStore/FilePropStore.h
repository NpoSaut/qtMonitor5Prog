﻿#ifndef SIMPLEFILEPROPSTORE_H
#define SIMPLEFILEPROPSTORE_H

#include <QFile>
#include <QMap>

#include "PropStore.h"

// Словарь свойст, хранящийся в простом текстовом файле
//  Каждая строка файла - запись. Содержит ключь и значение, разделённые пробелом.
//  Значения кешируются в памяти.
class FilePropStore : public PropStore
{
public:
    FilePropStore(QFile &storeFile);

    // Выводит список всех хранимых ключей
    virtual QList<quint8> keys() const;

    // Возаращает все хранимые данные
    virtual QVector< QPair<quint8, qint32> > data() const;

    // Считать свойство по ключу
    //  Значения возвращается по ссылке, успешность по return
    virtual bool get (quint8 key, qint32 &value) const;

    // Записать свойство по ключу
    //  В случае отсутсвия, свойство добавляется. Успешность по return
    virtual bool set (quint8 key, qint32 value);

    // Удаляет свойство
    virtual bool del (quint8 key);

    // Синхронизирует данные в кэше и файле.
    bool sync ();

    // Обновляет локальную версию данных из файла (не записанные изменения теряются)
    virtual bool discard ();

private:
    QFile &file;
    QMap<quint8, qint32> map;
};

#endif // SIMPLEFILEPROPSTORE_H