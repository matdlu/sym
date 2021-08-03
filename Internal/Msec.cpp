#include "Msec.h"
extern "C" {
  #include <sodium.h>
}
#include <QByteArray>
#include <QString>

void* msecAlloc(int count, int size) {
    void* ptr = sodium_allocarray(count, size);
    sodium_mlock(ptr, count*size);
    return ptr;
}

void msecFree(void* ptr, int length) {
    sodium_munlock(ptr, length); // automatically zeroes the area
    sodium_free(ptr);
    //ptr = 0;
}

void msecFree(QByteArray* array) {
    array->fill(0);
    array->clear();
    delete array;
    //array = 0;
}

void msecFree(QByteArray array) {
    array.fill(0);
    array.clear();
}

void msecFree(QString* string) {
    string->fill(0);
    string->clear();
    delete string;
    //string = 0;
}

MsecArray* newMsecArray(int size) {
    return new MsecArray { (char*)msecAlloc(size, 1), size };
}

void msecFree(MsecArray* array) {
    msecFree(array->data, array->length); // automatically zeroes the area
    *array = { 0, 0 };
    delete array;
    //array = 0;
}

#ifdef QT_DEBUG
QByteArray* msecToQt(MsecArray* array) {
    return new QByteArray(array->data, array->length);
}
QString msecToBase64(MsecArray* array) {
    return msecToQt(array)->toBase64();
}
#endif
