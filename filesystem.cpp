#include "filesystem.h"

FileSystem::FileSystem() : root({}, {}, FileType::directory), content(), parent(nullptr)
{
}

FileSystem::FileSystem(File _root, FileSystem* _parent) : root(_root), content(), parent(_parent)
{
}

FileSystem::~FileSystem()
{
    size_t contentSize = content.size();
    for (size_t i = 0; i < contentSize; ++i)
    {
        delete content[i];
    }
}

std::string FileSystem::pwd() const
{
    if(!parent) return "/";
    if(!parent->parent) return "/" + root.getName();
    return parent->pwd() + "/" + root.getName();
}

FileSystem* FileSystem::cd(std::string directory)
{
    if(directory[0] == '/' && parent) return parent->cd(directory);
    if(directory[0] == '/' && !parent)
    {
        if(directory.size() > 1) return cd(directory.erase(0,1));
        return this;
    }
    if(directory[0] == '.' && directory[1] == '.')
    {
        if(directory[2] == '/')
        {
            if(parent) return parent->cd(directory.erase(0,3));
            return cd(directory.erase(0,3));
        }
        if(parent)
        {
            parent->root.updateLastAccess();
            return parent;
        }
        return this;
    }
    if(directory[0] == '.')
    {
        if(directory[1] == '/') return cd(directory.erase(0,2));
        return this;
    }
    size_t firstSlash = directory.find('/');
    std::string toFind;
    size_t contentSize = content.size();
    if(firstSlash == std::string::npos)
    {
        toFind = directory;
        for (size_t i = 0; i < contentSize; ++i)
        {
            if(content[i]->root.getType() == FileType::directory && content[i]->root.getName() == toFind)
            {
                content[i]->root.updateLastAccess();
                return content[i];
            }
        }
        return nullptr;
    }
    else
    {
        toFind = directory.substr(0, firstSlash);
        for (size_t i = 0; i < contentSize; ++i)
        {
            if(content[i]->root.getType() == FileType::directory && content[i]->root.getName() == toFind) return content[i]->cd(directory.erase(0, firstSlash + 1));
        }
        return nullptr;
    }
}

std::vector<std::string> FileSystem::ls(std::string path)
{
    std::vector<std::string> result;
    if(path == "")
    {
        size_t contentSize = content.size();
        for (size_t i = 0; i < contentSize; ++i)
        {
            result.push_back(content[i]->root.getName());
        }
        return result;
    }
    FileSystem* fs = cd(path);
    if(fs != nullptr)
    {
        size_t contentSize = fs->content.size();
        for (size_t i = 0; i < contentSize; ++i)
        {
            result.push_back(fs->content[i]->root.getName());
        }
    }
    return result;
}

std::string FileSystem::cat(std::vector<std::string>& inputPaths, std::string outputPath, std::string accRes = "")
{
    if(inputPaths.size() == 0)
    {
        if(outputPath != "")
        {
            FileSystem* fs = cd(outputPath);
            if(fs != nullptr)
            {
                fs->root.updateLastContentChange();  
                fs->root.setFileContent(accRes); 
            }
            else
            {
                size_t lastSlash = outputPath.find_last_of('/');
                if(lastSlash != std::string::npos && lastSlash != 0)
                {
                    std::string s = outputPath.substr(0, lastSlash);
                    fs = cd(s);
                    File file(outputPath.substr(lastSlash + 1), accRes, FileType::regular);
                    fs->content.push_back(new FileSystem(file, fs));
                }
                else
                {
                    if(lastSlash == 0) outputPath.erase(0,1);
                    File file(outputPath, accRes, FileType::regular);
                    content.push_back(new FileSystem(file, this));
                }
            }
        }
        return accRes;
    }
    FileSystem* fs = cd(inputPaths[0]);
    if(fs != nullptr)
    {
        accRes += fs->root.getFileContent();
        inputPaths.erase(inputPaths.begin());
        return cat(inputPaths, outputPath, accRes);
    }
    return accRes;
}

void FileSystem::cp(std::vector<std::string>& fromPaths, std::string toPath)
{
    size_t numToCopy = fromPaths.size();
    for (size_t i = 0; i < numToCopy; ++i)
    {
        std::vector<std::string> v;
        v.push_back(fromPaths[i]);
        size_t ind = fromPaths[i].find_last_of('/');
        if(ind != std::string::npos)
        {
            cat(v, toPath + fromPaths[i].substr(ind));
        }
        else 
        {
            cat(v, toPath + '/' + fromPaths[i]);
        }
    }
}

void FileSystem::rm(std::vector<std::string>& paths)
{
    size_t numToDel = paths.size();
    for (size_t i = 0; i < numToDel; ++i)
    {
        size_t ind = paths[i].find_last_of('/');
        if(ind != std::string::npos && ind != 0)
        {
            FileSystem* fs = cd(paths[i].substr(0, ind));
            size_t contentSize = fs->content.size();
            for (size_t j = 0; j < contentSize; ++j)
            {
                if(fs->content[j]->root.getType() == FileType::regular && fs->content[j]->root.getName() == paths[i].substr(ind + 1))
                {
                    delete fs->content[j];
                    fs->content.erase(fs->content.begin()+j);
                    --j;
                    --contentSize;
                }
            }
        }
        else
        {
            if(ind == 0) paths[i].erase(0,1);
            size_t contentSize = content.size();
            for (size_t j = 0; j < contentSize; ++j)
            {
                if(content[j]->root.getType() == FileType::regular && content[j]->root.getName() == paths[i])
                {
                    delete content[j];
                    content.erase(content.begin()+j);
                    --j;
                    --contentSize;
                }
            }
            
        }
    }
    
}

void FileSystem::mkdir(std::vector<std::string>& paths)
{
    size_t numToCreate = paths.size();
    for (size_t i = 0; i < numToCreate; ++i)
    {
        size_t lastSlash = paths[i].find_last_of('/');
        if(lastSlash == std::string::npos)
        {
            File file(paths[i], {}, FileType::directory);
            content.push_back(new FileSystem(file, this));
        }
        else
        {
            std::string s = paths[i].substr(0, lastSlash);
            FileSystem* fs = cd(s);
            File file(paths[i].substr(lastSlash + 1), {}, FileType::directory);
            fs->content.push_back(new FileSystem(file, fs));
        }
    }
}

void FileSystem::rmdir(std::vector<std::string>& paths)
{
    size_t numToCDelete = paths.size();
    for (size_t i = 0; i < numToCDelete; ++i)
    {
        size_t lastSlash = paths[i].find_last_of('/');
        std::string s = paths[i];
        if(lastSlash != std::string::npos && lastSlash != 0)
        {
            s = paths[i].substr(0, lastSlash);
        }
        FileSystem* fs = cd(s);
        std::string toFind = paths[i].substr(lastSlash + 1);
        if(fs->content.size() == 0) {
            
            size_t ps = fs->parent->content.size();
            for (size_t j = 0; j < ps; ++j)
            {
                if(fs->parent->content[j]->root.getType() == FileType::directory && fs->parent->content[j]->root.getName() == toFind)
                {
                    fs->parent->content.erase(fs->parent->content.begin()+j);
                    delete fs;
                }
            }
        }
    }
}

void FileSystem::ln()
{
}

std::vector<std::vector<time_t>> FileSystem::stat(std::vector<std::string> paths)
{
    std::vector<std::vector<time_t>> res;
    size_t numFiles = paths.size();
    for (size_t i = 0; i < numFiles; ++i)
    {
        std::vector<time_t> curr;
        FileSystem* fs = cd(paths[i]);
        curr.push_back(fs->root.getLastAccess());
        curr.push_back(fs->root.getLastContentChange());
        curr.push_back(fs->root.getLastMetadataChange());
        res.push_back(curr);
    }
    return res;
}

std::string FileSystem::getRootName() const
{
    return root.getName();
}