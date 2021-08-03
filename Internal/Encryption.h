#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <QByteArray>
#include <Msec.h>
#include <QObject>

QByteArray encrypt(const char* msg, int msg_l, const char* key);
MsecArray* decrypt(QByteArray* cipherText, const char* key);

#endif // ENCRYPTION_H
