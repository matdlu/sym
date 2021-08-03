#ifndef ARGS_H
#define ARGS_H

#include <Global.h>
#include <QString>

struct Args {
    Command cmd = HELP;
    QString path = "";
    QString encFilePath = "";
};

Args parseArgs(int argc, char* argv[]);
void act(Args args);

#endif // ARGS_H
