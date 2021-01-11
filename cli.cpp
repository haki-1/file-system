#include "cli.h"

CommandLineInterface::CommandLineInterface() : endState(false)
{
}

CommandLineInterface::~CommandLineInterface()
{
}

void CommandLineInterface::setCommand(std::string _command)
{
    command = _command;
}

void CommandLineInterface::executeCommand(FileSystem*& fs)
{
    if(command == "pwd") pwd(fs);
    if(command == "cd") cd(fs);
    if(command == "ls") ls(fs);
    if(command == "cat") cat(fs);
    if(command == "cp") cp(fs);
    if(command == "rm") rm(fs);
    if(command == "mkdir") mkdir(fs);
    if(command == "rmdir") rmdir(fs);
    if(command == "ln") ln(fs);
    if(command == "stat") stat(fs);
    if(command == "exit") exit(fs);
}

void CommandLineInterface::pwd(FileSystem*& fs) const
{
    std::cout << fs->pwd() << std::endl;
}

void CommandLineInterface::cd(FileSystem*& fs) const
{
    std::string s1;
    std::getline(std::cin, s1);
    s1.erase(0, 1);
    fs = fs->cd(s1);
}

void CommandLineInterface::ls(FileSystem*& fs) const
{
    std::string s1;
    std::getline(std::cin, s1);
    s1.erase(0, 1);
    std::vector<std::string> v = fs->ls(s1);
    size_t elems = v.size();
    for (size_t i = 0; i < elems; ++i)
    {
        std::cout << v[i] << " ";
    }
}

void CommandLineInterface::cat(FileSystem*& fs) const
{
    std::vector<std::string> v1;
    std::string s1, s2;
    std::string s3 = "";
    std::getline(std::cin, s1);
    std::stringstream ss(s1);
    bool flag = false;
    while(ss >> s1)
    {
        if(s1 != ">")
        {
            v1.push_back(s1);
        }
        else
        {
            ss >> s1;
            s2 = s1;
            flag = true;
        }
    }

    if(v1.empty())
    {
        std::getline(std::cin, s1);
        while (s1 != ".")
        {
            s3 += s1;
            std::getline(std::cin, s1);
        }
        fs->cat(v1, s2, s3);
    }
    else if(!flag)
    {
        std::cout << fs->cat(v1, "", "");
    }
    else fs->cat(v1, s2, "");
}

void CommandLineInterface::cp(FileSystem*& fs) const
{
    std::vector<std::string> v;
    std::string s1, s2;
    std::getline(std::cin, s1);
    std::stringstream ss(s1);
    ss >> s2;
    while(ss >> s1)
    {
        v.push_back(s2);
        s2 = s1;
    }
    std::cout << std::endl<< s1 << std::endl << s2 << std::endl;
    fs->cp(v,s2);
}

void CommandLineInterface::rm(FileSystem*& fs) const
{
    std::vector<std::string> v;
    std::string s1;
    std::getline(std::cin, s1);
    std::stringstream ss(s1);
    while(ss >> s1)
    {
        v.push_back(s1);
    }
    fs->rm(v);
}

void CommandLineInterface::mkdir(FileSystem*& fs) const
{
    std::vector<std::string> v;
    std::string s1;
    std::getline(std::cin, s1);
    std::stringstream ss(s1);
    while(ss >> s1)
    {
        v.push_back(s1);
    }
    fs->mkdir(v);
}
void CommandLineInterface::rmdir(FileSystem*& fs) const
{
    std::vector<std::string> v;
    std::string s1;
    std::getline(std::cin, s1);
    std::stringstream ss(s1);
    while(ss >> s1)
    {
        v.push_back(s1);
    }
    fs->rmdir(v);
}

void CommandLineInterface::ln(FileSystem*& fs) const
{
}

void CommandLineInterface::stat(FileSystem*& fs) const
{
    std::vector<std::string> v;
    std::string s1;
    std::getline(std::cin, s1);
    std::stringstream ss(s1);
    while(ss >> s1)
    {
        v.push_back(s1);
    }
    std::vector<std::vector<time_t>> res = fs->stat(v);
    size_t sizeV = v.size();
    for (size_t i = 0; i < sizeV; ++i)
    {
        std::cout << fs->cd(v[i])->getRootName() << std::endl;
        std::cout << "Last Access: " << ctime(&res[i][0]);
        std::cout << "Last Content Change: " << ctime(&res[i][1]);
        std::cout << "Last Metadata Change: " << ctime(&res[i][2]) << std::endl;
    }
}

void CommandLineInterface::exit(FileSystem*& fs)
{
    endState = true;
}

bool CommandLineInterface::getEndState() const
{
    return endState;
}