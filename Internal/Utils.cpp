#include <Utils.h>
#include <QFile>
#include <QDebug>

int saveByteArrayToFile(QString fileName, QByteArray* array) {
    QFile file(fileName);
    if ( ! file.open(QIODevice::WriteOnly) ) {
        qDebug() << "Couldn't save the array to a file:" << fileName;
        return -1;
    }
    if ( file.write(*array) != array->length() ) {
        qDebug() << "Not the whole array was written.";
        file.close();
        return -1;
    }
    file.close();
    return 0;
}

QByteArray* loadByteArrayFromFile(QString fileName) {
    QFile file(fileName);
    if ( ! file.open(QIODevice::ReadOnly) ) {
        qDebug() << "Couldn't open the file:" << fileName;
        return 0;
    }
    if ( ! file.isReadable() ) {
        qDebug() << "Cannot read the file where array is stored.";
        file.close();
        return 0;
    }
    QByteArray* array = new QByteArray();
    *array = file.readAll();
    file.close();
    return array;
}
