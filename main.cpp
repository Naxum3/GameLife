#include "Ruller.h"

int main(int argc, char* argv[]) {
    Ruller law;
    law.read_argv(argc, argv);
    law.read_file();
    law.start();
    return 0;
}