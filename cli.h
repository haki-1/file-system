#ifndef __CLI_H
#define __CLI_H

#include "filesystem.h"
#include <string>
#include <sstream>
#include <iostream>


class CommandLineInterface
{
private:
    std::string command;
    bool endState;

    void pwd(FileSystem*&) const;
    void cd(FileSystem*&) const;
    void ls(FileSystem*&) const;
    void cat(FileSystem*&) const;
    void cp(FileSystem*&) const;
    void rm(FileSystem*&) const;
    void mkdir(FileSystem*&) const;
    void rmdir(FileSystem*&) const;
    void ln(FileSystem*&) const;
    void stat(FileSystem*&) const;
    void exit(FileSystem*&);

public:
    CommandLineInterface();
    ~CommandLineInterface();

    void setCommand(std::string);
    void executeCommand(FileSystem*&);
    bool getEndState() const;
};

#endif