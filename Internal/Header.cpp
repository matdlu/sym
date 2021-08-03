#include <Header.h>
#include <Global.h>
#include <QDebug>

const int header_buffer_length = 128;
const char header_seperator = '|';

/* Defines the order of appearance of individual fields. */
enum HeaderOrder {
    header_version,
    header_saltLength,
    header_salt, // base64 encoded
    header_memory,
    header_iterations,
    header_parallelism,
    header_hashLength,
    header_nfields,
};

Header readHeader(QByteArray* data) {
    if ( data->at(0) != header_seperator ) {
        qDebug() << "File is propably not a sym encrypted file. It should begin with:" << header_seperator << "character but doesn't. Quitting.";
        exit(-1);
    }
    Header header;
    int sep_cnt = 0;
    int last_index = 0;
    for(int i = 0; sep_cnt < header_nfields && i < data->length(); i++) {
        if ( data->at(i) == header_seperator ) {
            if ( i > 0 ) { // ignore first seperator
                int begin = last_index+1;
                int end =  i-last_index-1;
                QString value = QString::fromUtf8(data->mid(begin, end));
#ifdef QT_DEBUG
                qDebug() << "HEADER: " << value << " i:" << i << "begin:" << begin << "end:" << end << "sep_cnt:" << sep_cnt;
#endif
                switch(sep_cnt) {
                    case header_version:     header.version            = value; break;
                    case header_saltLength:  header.params.saltLength  = value.toInt(); break;
                    case header_salt:        header.salt               = QByteArray::fromBase64(value.toUtf8()); break;
                    case header_memory:      header.params.memory      = value.toInt(); break;
                    case header_iterations:  header.params.iterations  = value.toInt(); break;
                    case header_parallelism: header.params.parallelism = value.toInt(); break;
                    case header_hashLength:  header.params.hashLength  = value.toInt(); break;
                }
                sep_cnt++;
            }
            last_index = i;
        }
    }
    header.length = last_index + 1;
    return header;
}

QByteArray createHeader(QByteArray* salt, Argon2Params argonParams) {
    QByteArray header;
    header.reserve(header_buffer_length);

    header.append(header_seperator);

    header.append(version);
    header.append(header_seperator);

    header.append(QString::number(argonParams.saltLength).toUtf8());
    header.append(header_seperator);

    header.append(salt->toBase64());
    header.append(header_seperator);

    header.append(QString::number(argonParams.memory).toUtf8());
    header.append(header_seperator);

    header.append(QString::number(argonParams.iterations).toUtf8());
    header.append(header_seperator);

    header.append(QString::number(argonParams.parallelism).toUtf8());
    header.append(header_seperator);

    header.append(QString::number(argonParams.hashLength).toUtf8());
    header.append(header_seperator);

    return header;
}
