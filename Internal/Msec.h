#ifndef MSEC_H
#define MSEC_H

#include <QByteArray>
#include <QObject>

void* msecAlloc(int count, int size);
void msecFree(void* ptr, int length);
void msecFree(QByteArray* array);
void msecFree(QByteArray array);
void msecFree(QString* string);

/* Data structure to use when QString or QByteArray are too big to be trusted. */
struct MsecArray {
    char* data;
    int length;
};
MsecArray* newMsecArray(int size);
void msecFree(MsecArray* array);
#ifdef QT_DEBUG
QByteArray* msecToQt(MsecArray* array);
QString msecToBase64(MsecArray* array);
#endif

#endif // MSEC_H
