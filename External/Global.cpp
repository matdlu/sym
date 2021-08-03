#include <Global.h>

const char* version = "1.0";
const char* extension = ".sym";
const char* helpStr =
"sym - symmetric encryption tool\n"
"Usage:\n"
"sym e* file - prompts for passphrase and produces file.sym encrypted file.\n"
"sym d* file.sym - prompts for passphrase and prints file contents to stdout.\n"
"sym h* - display help\n"
"It's possible to supply password through stdin (e.g. echo password | sym e file).";
