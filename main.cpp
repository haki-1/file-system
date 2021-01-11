#include "cli.h"
#include "file.h"
#include "filesystem.h"

int main()
{
    CommandLineInterface cli;
    FileSystem* fileSystem = new FileSystem();
    std::string command;
    while (!cli.getEndState())
    {
        std::cin >> command;
        cli.setCommand(command);
        cli.executeCommand(fileSystem);
    }
    return 0;
}