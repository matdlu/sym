#ifndef HASHING_H
#define HASHING_H

#include <Msec.h>
#include <QObject>

struct Argon2Params {
    int memory;
    int iterations;
    int parallelism;
    int saltLength;
    int hashLength;
};

extern const Argon2Params defaultArgon2;

QByteArray generateSalt(int saltLength = defaultArgon2.saltLength);

/* pw - password, it should be zeroed after getKey is called! It needs to be exactly of pw_l length (no \0 characters at the end or something) otherwise sodium_memzero will crash for some reason.
 * pw_l - passsword length
 * Returns a pointer to hash it should be zeroed and freed with sodium_free! */
MsecArray* keyFromPassword(const char* pw, int pw_l, char* salt, Argon2Params params = defaultArgon2);

#endif // HASHING_H
