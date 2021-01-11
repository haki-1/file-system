#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H

#include "file.h"
#include <iostream>
#include <vector>

class FileSystem
{
private:
    File root;
    std::vector<FileSystem*> content;
    FileSystem* parent;
public:
    FileSystem();
    FileSystem(File, FileSystem*);
    ~FileSystem();

    std::string pwd() const;
    FileSystem* cd(std::string);
    std::vector<std::string> ls(std::string);
    std::string cat(std::vector<std::string>&, std::string, std::string);
    void cp(std::vector<std::string>&, std::string);
    void rm(std::vector<std::string>&);
    void mkdir(std::vector<std::string>&);
    void rmdir(std::vector<std::string>&);
    void ln();
    std::vector<std::vector<time_t>> stat(std::vector<std::string>);
    std::string getRootName() const;
};

#endif