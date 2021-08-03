#include <Command.h>
#include <Header.h>
#include <Encryption.h>
#include <Utils.h>
#include <Msec.h>
#include <Global.h>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <stdio.h>
/* for setting binary mode */
#ifdef _WIN32
# include <io.h>
# include <fcntl.h>
# define SET_BINARY_MODE(handle) setmode(handle, O_BINARY)
#else
# define SET_BINARY_MODE(handle) freopen(NULL, "rb", stdin)
#endif
/* for setting echo on terminal */
#ifdef WIN32
# include <windows.h>
#else
# include <termios.h>
#endif

#define DISABLE_ECHO

static const int password_buffer_length = 192;

void setEchoStdin(bool enable = true)
{
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if( ! enable )
        mode &= ~ENABLE_ECHO_INPUT;
    else
        mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode);
#else
    struct termios tty;
    tcgetattr(fileno(stdin), &tty);
    if( ! enable )
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    tcsetattr(fileno(stdin), TCSANOW, &tty);
#endif
}

QString* askForPassword() {
    fprintf(stderr, "Enter passphrase: ");
    QString* pw = new QString();
    pw->reserve(password_buffer_length);
#ifdef DISABLE_ECHO
    setEchoStdin(false);
#endif
    QTextStream(stdin).readLineInto(pw);
#ifdef DISABLE_ECHO
    setEchoStdin(true);
    fputc('\n', stderr); // Needs to output to something else than stdout for > operator to work properly (to not add newline to the resulting file)
#endif
    return pw;
}

MsecArray* askAndGenerateKey(QByteArray* salt, Argon2Params params) {
    // ask for password
    QString* pw = askForPassword();

    // hash the pasword
    MsecArray* key = keyFromPassword(pw->toUtf8(), pw->length(), salt->data(), params);

    // clear password
    msecFree(pw);

    return key;
}

void encryptCmd(Args args) {
    Argon2Params params = defaultArgon2;

    // load the file to memory
    QByteArray* file_data = loadByteArrayFromFile(args.path);
    if ( file_data == 0 ) exit(-1);

    // generate salt
    QByteArray salt = generateSalt(params.saltLength);

    // get key from user
    MsecArray* key = askAndGenerateKey(&salt, params);

#ifdef QT_DEBUG
    qDebug() << "key" << msecToBase64(key);
#endif

    // encrypt the file with a given password in memory
    QByteArray data = encrypt(file_data->data(), file_data->length(), key->data);

    // clear the key and unencrypted data
    msecFree(key);
    msecFree(file_data);

#ifdef QT_DEBUG
    qDebug() << "data" << data.toBase64();
#endif

    // prepend the header
    QByteArray header = createHeader(&salt, params);
    data.prepend(header);

    // save the file to disk
    saveByteArrayToFile(args.encFilePath, &data);
}

void decryptCmd(Args args) {
    // load the file to memory
    QByteArray* file_data = loadByteArrayFromFile(args.path);
    if ( file_data == 0 ) exit(-1);

    // read the header
    Header header = readHeader(file_data);

    // cut the header from data
    QByteArray encrypted_data = file_data->mid(header.length);

#ifdef QT_DEBUG
    qDebug() << "Header information:";
    qDebug() << "version " << header.version;
    qDebug() << "saltLength " << header.params.saltLength;
    qDebug() << "salt" << header.salt.toBase64();
    qDebug() << "memory" << header.params.memory;
    qDebug() << "iterations" << header.params.iterations;
    qDebug() << "parallelism" <<header.params.parallelism;
    qDebug() << "hashLength " << header.params.hashLength;
    qDebug() << "header length " << header.length;
#endif

    // ask for password
    MsecArray* key = askAndGenerateKey(&header.salt, header.params);

#ifdef QT_DEBUG
    qDebug() << "key" << msecToBase64(key);
    qDebug() << "data" << encrypted_data.toBase64();
#endif

    // decrypt
    MsecArray* data = decrypt(&encrypted_data, key->data);

    // clear the key
    msecFree(key);

    // write to stdout in binary mode
    SET_BINARY_MODE(stdout);
    fwrite(data->data, 1, data->length, stdout);

    // clear decrypted data
    msecFree(data);

    // clear memory
    msecFree(file_data);
}

void helpCmd() {
    puts(helpStr);
}
