#include "proglist.h"

namespace FudpMessage
{
ProgList::ProgList()
{
}

ProgList::ProgList(QList<DevFileInfo> &listDevFileInfo) :
    listDevFileInfo(listDevFileInfo)
{
}

std::vector<byte> ProgList::encode()
{
    QByteArray buffer;
    QDataStream out(&buffer, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    out << (byte)MessageId(progList);
    int i = 0;
    foreach (DevFileInfo file, listDevFileInfo)
    {
        if ( buffer.size ()
             + (1 + file.getFileNameSize () + 4 + 4)                // Размер записи о добавляемом файле
             +( (i == listDevFileInfo.size()-1) ? 0 : (1 + 4 + 4) )    // Если файл не последний, то размер флага остатка
             <= maxSublevelDatagramLength )
       {
            i ++;
            out << file.getFileNameSize ();
            QByteArray t = Message::changeCodec(file.getFileName (), "Windows-1251");
            for (int i =0; i < t.size(); i ++)
                out << (quint8)t.at(i);

            out << file.getFileSize() << (quint32) file.getControlSum();
        }
        else
        {
            out << (byte) 0 << (quint32) (listDevFileInfo.size () - i) << (quint32) 0;
            break;
        }
    }

    return Message::fromQByteArrayToVector(buffer);
}

void ProgList::decode(const std::vector<byte> &data)
{
    listDevFileInfo.clear ();
    remainder = false;

    QByteArray buffer = Message::fromVectorToQByteArray(data);
    QDataStream in(&buffer, QIODevice::ReadOnly);
    in.setByteOrder(QDataStream::LittleEndian);

    if (buffer.size () >= 2)
    {
        qint8 fNameSize = buffer[1];
        for(int i = 1; i < buffer.size(); i + (1 + fNameSize + 4 + 4))
        {
            in >> fNameSize;
            if ( fNameSize != 0 )
            {
                char *temp = new char[fNameSize];
                in.readRawData(temp, fNameSize);
                QString fName(temp);
                delete [] temp;

                qint32 fSize;
                in >> fSize;

                qint32 controlSum;
                in >> controlSum;

                DevFileInfo dfi(fName, fSize, controlSum);
                listDevFileInfo.append (dfi);
            }
            else // Признак наличия файлов, не вошедших в сообщение
            {
                remainder = true;
                break;
            }
        }
    }
}
}
