#include<iostream>
#include<fstream>
#include<string>
#include<dirent.h>
#include<windows.h>
#include"DisplayInfo.h"

const int MAX_FILE_NO = 40;

using namespace std;

class DirectoryHandler
{
protected:
    const char *dirPath;
    string filename[MAX_FILE_NO];
    int fileNo;
    string FilePath;

public:
    bool isValid = true;
     DisplayInfo dirDi;

    void createDir()
    {
        string mkdirCommand = string("mkdir ") + string(dirPath);
        system(mkdirCommand.data());
    }

    void setDirName(const char *dirpath)
    {
        dirPath = dirpath;
    }

    void setDirForUse()
    {
        setFileNames();
    }

    bool exists()
    {
        DIR* dir = opendir(dirPath);
        if (dir){
            closedir(dir);
            return true;
        }
          else {
            return false;
        }
    }

    void removeFile(int i)
    {
        FilePath = string(dirPath) + filename[i];
        remove(FilePath.data());
    }

    void openFileExternal(int i)
    {
        string command = string("start ") + string(dirPath) + filename[i];
        system(command.data());
    }

    void hideFile(int i)
    {
        FilePath = string(dirPath) + filename[i];
        char attributes = GetFileAttributes(FilePath.data());
        if(!(attributes & FILE_ATTRIBUTE_HIDDEN))   //bitwise and to check if file is not hidden
            SetFileAttributes(FilePath.data(), attributes + FILE_ATTRIBUTE_HIDDEN);
    }

    void unhideFile(int i)
    {
        FilePath = string(dirPath) + filename[i];
        char attributes = GetFileAttributes(FilePath.data());
        if(attributes & FILE_ATTRIBUTE_HIDDEN)    //bitwise and to check if file is hidden
            SetFileAttributes(FilePath.data(), attributes - FILE_ATTRIBUTE_HIDDEN);
    }

    void hidedir()
    {
        char attributes = GetFileAttributes(dirPath);
        if(!(attributes & FILE_ATTRIBUTE_HIDDEN))  //bitwise and to check if directory is not hidden
            SetFileAttributes(dirPath, attributes + FILE_ATTRIBUTE_HIDDEN);
    }

    void unhidedir()
    {
        char attributes = GetFileAttributes(dirPath);
        if(attributes & FILE_ATTRIBUTE_HIDDEN)     //bitwise and to check if directory is hidden
            SetFileAttributes(dirPath, attributes - FILE_ATTRIBUTE_HIDDEN);
    }

    string getFileExtension(string fileName)
    {
        string ext("");
        bool flag = false;
        int extcount = 0;
        int pos = fileName.find(".");
        if(pos != -1)
        {
        for(unsigned int a = pos;a<fileName.size();a++)
        {
            ext.push_back(fileName[a]);
        }
        }
        return ext;
    }

    int getFileNumber()
    {
        return fileNo;
    }

    string getFileName(int i)
    {
        return filename[i];
    }

    void setFileNames()
    {
        DIR *dir;
        struct dirent *ent;
        dir = opendir(dirPath);
        if (dir == 0)
        {
            isValid = false;
        }
        int i = 0;
        int fileCount =0;

        while((ent = readdir(dir))!= NULL)
        {
            if(i>1)
            {
                filename[i-2] = ent->d_name;
                fileCount++;
            }
            i++;
        }
        fileNo = fileCount;

        if(i<MAX_FILE_NO)
        {
            for(int j=i;j<MAX_FILE_NO;j++)
            {
                filename[j] = "\0";
            }
        }
        closedir(dir);
    }

    void displayfile()
    {
        for(int i=0;i<fileNo;i++)
        {
            if (filename[i] == "\0")
                break;
            dirDi.add(filename[i]);
        }
    }
};
