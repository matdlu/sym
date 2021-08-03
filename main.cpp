#include <QDebug>
#include <Args.h>
extern "C" {
  #include <sodium.h>
}

int main(int argc, char *argv[])
{
    if ( sodium_init() < 0 ) {
        qDebug() << "Libsodium couldn't be initialized.";
        exit(0);
    }

    act(parseArgs(argc, argv));
}
