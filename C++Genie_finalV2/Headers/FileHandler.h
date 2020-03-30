
#include<fstream>

class FileHandler
{
protected:
    const char* filePath;
    fstream file;
public:
    void setFilePath(const char* fpath)
    {
        filePath = fpath;
    }

    void moveFileTo(char* newpath)
    {
        rename(filePath,newpath);
    }

    void createFile()
    {
        file.open(filePath ,ios::out);
        if(!file)
        {
            filePath = "";
            std::cout<<"\nError creating file";
        }
        file.close();
    }

    void removeFile()
    {
        remove(filePath);
    }

    void openFileExternal()
    {
        string command = string("start ") + string(filePath);
        system(command.data());
    }

    bool exists ()
    {
    file.open(filePath,ios::in);
    bool check = file.good();
    file.close();
    return check;
    }

};
