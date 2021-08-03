#ifndef HEADER_H
#define HEADER_H
#include <QString>
#include <QByteArray>
#include <Hashing.h>

extern const int header_buffer_length;
extern const char header_seperator;

struct Header {
    QString version;
    QByteArray salt;
    Argon2Params params;
    int length;
};

Header readHeader(QByteArray* data);
QByteArray createHeader(QByteArray* salt, Argon2Params argonParams);

#endif // HEADER_H
