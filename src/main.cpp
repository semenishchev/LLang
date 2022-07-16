#include "main.h"
#include <sys/stat.h>

long getFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}


int main(int argc, char* argv[]) {
    // TODO:
    // read input file
    // tokenize and parse it
    // generate LLVM IR
    // compiler generated LLVM IR
    cout << argc << " " << *argv[1] << "\n";

    //Lexer::test();

    return 0;
}