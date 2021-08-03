#include <Hashing.h>
#include <Utils.h>
extern "C" {
  #include <argon2.h>
  #include <sodium.h>
}
#include <QString>
#include <QByteArray>
#include <QDebug>

/*                                   memory, iterations, parallelism, saltLength, hashLength */
const Argon2Params defaultArgon2 = { 524288, 3,          8,           32,         32 };

QByteArray generateSalt(int saltLength) {
    int arr_l = saltLength;
    char* arr = (char*)malloc(arr_l); // use malloc, because safety from side-channel attacks is not important for salts, which are stored in cleartext files anyways
    randombytes_buf(arr, arr_l);
    auto array = QByteArray(arr, arr_l);
    free(arr);
    return array;
}

MsecArray* keyFromPassword(const char* pw, int pw_l, char* salt, Argon2Params params) {
    int arr_l = params.hashLength;
    char* arr = (char*)msecAlloc(arr_l, 1);
    int r = argon2id_hash_raw(params.iterations, params.memory, params.parallelism, pw, pw_l, salt, params.saltLength, arr, params.hashLength);
    if ( r != ARGON2_OK ) {
        qDebug() << "Something went wrong, could not compute the hash, error code: " << r;
        return 0;
    }
    return new MsecArray { arr, arr_l };
}
