#include "file.h"

size_t File::numOfFiles = 0;

File::File(std::string _name, std::string _fileContent, FileType _type) : id(++numOfFiles), name(_name), fileContent(_fileContent), type(_type), size(_fileContent.size()), lastAccess(time(0)), lastContentChange(time(0)), lastMetadataChange(time(0))
{
}

File::~File()
{
}

std::string File::getName() const
{
    return name;
}

void File::setName(std::string s)
{
    name = s;
    return;
}

std::string File::getFileContent() const
{
    return fileContent;
}

void File::setFileContent(std::string s)
{
    fileContent = s;
}

FileType File::getType() const
{
    return type;
}

time_t File::getLastAccess() const
{
    return lastAccess;
}

time_t File::getLastContentChange() const
{
    return lastContentChange;
}

time_t File::getLastMetadataChange() const
{
    return lastMetadataChange;
}

void File::updateLastAccess()
{
    lastAccess = time(0);
    updateLastMetadataChange();
}

void File::updateLastContentChange()
{
    lastContentChange = time(0);
    updateLastMetadataChange();
}

void File::updateLastMetadataChange()
{
    lastMetadataChange = time(0);
}