#include "read_bin.h"

#include <unistd.h>

int main()
{
    ReadFile read_bin_file;

    int size = read_bin_file.GetBinaryFileSize();

    read_bin_file.ReadBinaryFlieAndSubpackage();

    read_bin_file.ShowTheFile();

    pause();
}