#ifndef __FILE_H
#define __FILE_H

#include <string>
#include <ctime>

enum FileType { regular, directory };


class File
{
private:
    const size_t id;
    size_t size;
    const FileType type;
    time_t lastAccess;
    time_t lastContentChange;
    time_t lastMetadataChange;
    std::string name;
    std::string fileContent;
    static size_t numOfFiles;

public:
    File(std::string, std::string, FileType);
    ~File();

    std::string getName() const;
    void setName(std::string);
    std::string getFileContent() const;
    void setFileContent(std::string);
    FileType getType() const;
    time_t getLastAccess() const;
    time_t getLastContentChange() const;
    time_t getLastMetadataChange() const;
    void updateLastAccess();
    void updateLastContentChange();
    void updateLastMetadataChange();
};

#endif