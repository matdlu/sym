#include <Encryption.h>
#include <Msec.h>
extern "C" {
  #include <sodium.h>
}
#include <QByteArray>
#include <QDebug>

//QByteArray encrypt(const char* msg, int msg_l, const char* key) {
//    int header_l = crypto_secretstream_xchacha20poly1305_HEADERBYTES;
//    char* header = new char[header_l];
//    crypto_secretstream_xchacha20poly1305_state state;
//    int r = crypto_secretstream_xchacha20poly1305_init_push(&state, (unsigned char*)header, (unsigned char*)key);
//    if ( r < 0 ) {
//        qDebug() << "Could not create the header, error code:" << r;
//        return 0;
//    }
//    int out_l = msg_l + crypto_secretstream_xchacha20poly1305_ABYTES; // "But with this particular construction, the ciphertext length is guaranteed to always be:"
//    char* out = new char[out_l];
//    r = crypto_secretstream_xchacha20poly1305_push(&state, (unsigned char*)out, 0, (unsigned char*)msg, msg_l, 0, 0, 0);
//    if ( r < 0 ) {
//        qDebug() << "Could not encrypt, error code:" << r;
//        return 0;
//    }
//    auto arr = QByteArray(out, out_l);
//    delete[] out;
//    arr.append(header, header_l);
//    delete[] header;
//    return arr;
//}

QByteArray encrypt(const char* msg, int msg_l, const char* key) {
    int header_l = crypto_secretstream_xchacha20poly1305_HEADERBYTES;
    char header[header_l];// = new char[header_l];
    crypto_secretstream_xchacha20poly1305_state state;
    int r = crypto_secretstream_xchacha20poly1305_init_push(&state, (unsigned char*)header, (unsigned char*)key);
    if ( r < 0 ) {
        qDebug() << "Could not create the header, error code:" << r;
        return 0;
    }
    int out_l = msg_l + crypto_secretstream_xchacha20poly1305_ABYTES; // "But with this particular construction, the ciphertext length is guaranteed to always be:"
    char* out = new char[out_l];
    r = crypto_secretstream_xchacha20poly1305_push(&state, (unsigned char*)out, 0, (unsigned char*)msg, msg_l, 0, 0, 0);
    if ( r < 0 ) {
        qDebug() << "Could not encrypt, error code:" << r;
        delete[] out;
        return 0;
    }
    auto arr = QByteArray(out, out_l);
    delete[] out;
    arr.append(header, header_l);
    //delete[] header;
    return arr;
}

MsecArray* decrypt(QByteArray* cipherText, const char* key) {
    int header_l = crypto_secretstream_xchacha20poly1305_HEADERBYTES;
    QByteArray header = cipherText->right(header_l);
    cipherText->remove(cipherText->length()-header_l, header_l);
    crypto_secretstream_xchacha20poly1305_state state;
    char* tmp =  header.data();
    int r = crypto_secretstream_xchacha20poly1305_init_pull(&state, (unsigned char*)tmp, (unsigned char*)key);
    if ( r < 0 ) {
        qDebug() << "Could not create the header, error code:" << r;
        return 0;
    }
    int out_l = cipherText->length() - crypto_secretstream_xchacha20poly1305_ABYTES;
    char* out = (char*)msecAlloc(out_l, 1);
    r = crypto_secretstream_xchacha20poly1305_pull(&state, (unsigned char*)out, 0, 0, (unsigned char*)cipherText->data(), cipherText->length(), 0, 0);
    if ( r < 0 ) {
        qDebug() << "Could not decrypt, error code:" << r;
        return 0;
    }
    return new MsecArray { out, out_l };
}
