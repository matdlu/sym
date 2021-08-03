#include <Args.h>
#include <Command.h>
#include <QDebug>
#include <QFile>

static const int cmd_index=1;
static const int path_index=2;

Args parseArgs(int argc, char* argv[]) {
    Args args;

    if ( argc < 2 ) {
        qInfo() << "At least one commandline argument is required.";
        args.cmd = HELP;
        return args;
    }

    switch(argv[cmd_index][0]) {
        case 'e': args.cmd = ENCRYPT; break;
        case 'd': args.cmd = DECRYPT; break;
        case 'h': args.cmd = HELP; break;
    }

    if ( args.cmd == ENCRYPT || args.cmd == DECRYPT ) {
        if ( argc < 3 ) {
            qInfo() << "For encrypt and decrypt commands at least two commandline arguments are required.";
            args.cmd = HELP;
            return args;
        }

        args.path = argv[path_index];
        if ( args.path.isEmpty() ) {
            qDebug() << "No path specified.";
            args.cmd = HELP;
            return args;
        }

        if (args.cmd == ENCRYPT ) {
            args.encFilePath = args.path + extension;
            if ( QFile::exists(args.encFilePath) ) {
                qDebug() << "There is a file on path:" << args.encFilePath + ". Will not overwrite.";
                exit(-1);
            }
        }
    }

    return args;
}


void act(Args args) {
    switch(args.cmd) {
        case ENCRYPT: encryptCmd(args); break;
        case DECRYPT: decryptCmd(args); break;
        case HELP: helpCmd(); break;
    }
}
