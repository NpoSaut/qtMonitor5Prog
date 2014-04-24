#include <QString>
#include <QtTest>
#include <QFile>

#include "../PropStore/FilePropStore.h"
#include "../PropStore/ConstLoaderStore.h"
#include "../PropStore/CompositePropStore.h"
#include "../PropStore/PropStoreDecorators.h"

class PropStoreTest : public QObject
{
    Q_OBJECT
    
public:
    PropStoreTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testFileStore();
    void testFileStore_data();

    void testKeyRange();
    void testKeyRange_data();

    void testConstLoaderStore();

private:
    void testBasePropStore (PropStore *store, quint8 key, qint32 value);

    void testKeysDataValueForExistKey (PropStore *store, quint8 key, qint32 value);
    void testKeysDataValueForAbsentKey (PropStore *store, quint8 key, qint32 value);

    QFile file;
    PropStore *fileStore;

    const int loaderVersion;
    const int loaderSubversion;
    const int protocolVersion;
    const int protocolSupportedVersion;
    PropStore *constStore;
//    PropStore *compositStore;

    static const quint8 leftBound = 10;
    static const quint8 rightBound = 200;
    PropStore *rangedStore;
};

PropStoreTest::PropStoreTest()
    : file ("testSimpleFileStore.txt"),
      loaderVersion (1),
      loaderSubversion(2),
      protocolVersion (3),
      protocolSupportedVersion (4)
{
}

void PropStoreTest::initTestCase()
{
    qsrand ((uint)QTime::currentTime ().msec ());

    file.remove ();
    file.open (QIODevice::WriteOnly);
    file.close ();
    fileStore = new FilePropStore (file);

    rangedStore = new PropStoreWithKeyRange (fileStore, leftBound, rightBound);

    constStore = new ConstLoaderStore (loaderVersion, loaderSubversion, protocolVersion, protocolSupportedVersion);
}

void PropStoreTest::cleanupTestCase()
{
    delete fileStore;
}

void PropStoreTest::testFileStore()
{
    QFETCH(QString, sKey); quint8 key = sKey.toInt ();
    QFETCH(QString, sValue); qint32 value = sValue.toInt ();

    testBasePropStore ( fileStore, key, value );
}

void PropStoreTest::testFileStore_data()
{
    QTest::addColumn<QString>("sKey");
    QTest::addColumn<QString>("sValue");

    for (int key = 0; key < 256; key ++)
    {
        qint32 value = (quint64 (quint16 (qrand ())) << 16) + quint16 (qrand ());
        QTest::newRow(
                    (QString("key: ") + QString::number (key) + QString(" ; value: ") + QString::number (value)).toAscii ()
                    ) << QString::number (key) << QString::number (value);
    }
}

void PropStoreTest::testKeyRange()
{
    rangedStore->discard ();

    QFETCH(QString, sKey); quint8 key = sKey.toInt ();
    QFETCH(QString, sValue); qint32 value = sValue.toInt ();

    if ( leftBound <= key && key <= rightBound )
    {
        testBasePropStore ( rangedStore, key, value );
    }
    else
    {
        rangedStore->set (key, value);
        testKeysDataValueForAbsentKey (rangedStore, key, value);
    }
}

void PropStoreTest::testKeyRange_data()
{
    QTest::addColumn<QString>("sKey");
    QTest::addColumn<QString>("sValue");

    QTest::newRow("out left bound")       << QString::number (leftBound-1)  << "100";
    QTest::newRow("in bound 1")           << QString::number (leftBound)    <<   "0";
    QTest::newRow("in bound 1 negative")  << QString::number (rightBound)   <<  "-1";
    QTest::newRow("out right bound")      << QString::number (rightBound+1) <<   "3";
}

void PropStoreTest::testConstLoaderStore()
{
    qint32 value;

    QVERIFY2( constStore->get (193, value), "Запрос версии зазрузчика" );
    QCOMPARE( value, loaderVersion );

    QVERIFY2( constStore->get (197, value), "Запрос подверсии зазрузчика" );
    QCOMPARE( value, loaderSubversion );

    QVERIFY2( constStore->get (195, value), "Запрос версии протокола" );
    QCOMPARE( value, protocolVersion );

    QVERIFY2( constStore->get (196, value), "Запрос поддерживаемой версии протокола" );
    QCOMPARE( value, protocolSupportedVersion );

    QVERIFY2( !constStore->set (1, 111), "Почему-то удалось установить свойство 1");

    QVERIFY( constStore->discard () );

    QVERIFY( constStore->sync () );
}

void PropStoreTest::testBasePropStore (PropStore *store, quint8 key, qint32 value)
{
    QVERIFY( store->set (key, value) );
    testKeysDataValueForExistKey (store, key, value);

    QVERIFY( store->sync () );
    testKeysDataValueForExistKey (store, key, value);


    QVERIFY( store->del (key));
    testKeysDataValueForAbsentKey (store, key, value);

    QVERIFY( store->discard () );
    testKeysDataValueForExistKey (store, key, value);
}

void PropStoreTest::testKeysDataValueForExistKey(PropStore *store, quint8 key, qint32 value)
{
    QVERIFY( store->keys ().indexOf (key) != -1 );

    QVERIFY( store->data ().indexOf (QPair<quint8, qint32> (key, value)) != -1 );

    qint32 getVal = value + 1;
    QVERIFY( store->get (key, getVal) );
    QCOMPARE( value, getVal );
}

void PropStoreTest::testKeysDataValueForAbsentKey(PropStore *store, quint8 key, qint32 value)
{
    QVERIFY( store->keys ().indexOf (key) == -1 );

    QVERIFY( store->data ().indexOf (QPair<quint8, qint32> (key, value)) == -1 );

    qint32 unchanged = value + 1;
    QVERIFY( store->get (key, unchanged) == false );
    QCOMPARE( value + 1, unchanged );
}

QTEST_APPLESS_MAIN(PropStoreTest)

#include "tst_propstoretest.moc"
