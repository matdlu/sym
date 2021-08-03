#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>

int saveByteArrayToFile(QString fileName, QByteArray* array);
QByteArray* loadByteArrayFromFile(QString fileName);

#endif // UTILS_H
