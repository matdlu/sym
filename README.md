# sym (symmetric encryption tool)

## About
Really just a very simple, quick and dirty encryption tool to encrypt a bunch of files. Use it at your own risk. However I use it daily without issues.

## Building
To build: (You need to have qbs installed)
```
qbs config:release 
```
To run:
```
cd default/install-root/usr/local/bin
./sym
```

## Crypto
Uses XChaCha20-Poly1305 from libsodium for encryption and argon2i from libargon2 for key derivation.

## Dependencies
- Qt
- libsodium (encryption and secure memory allocation)
- libargon2 (key derivation)

## Usage
```
sym e* file - prompts for passphrase and produces file.sym encrypted file.
sym d* file.sym - prompts for passphrase and prints file contents to stdout.
sym h* - display help
It's possible to supply password through stdin (e.g. echo password | sym e file). It's insecure due to many reasons but it's convieniant.
```

## Story
```
>Encrypted a bunch with gpg --symmetric
>Wanted to change the password for all of them.
>Couldn't supply the password through cmdline (--passphrase didn't work for some reason) to any of them.
>Had to decrypt all of them manually one by one.
>Looking for alternatives to not use gpg in the future, found age.
>But it also didn't allow for supplying the password through cmdline. 
>Decided to write my own tool.
```

