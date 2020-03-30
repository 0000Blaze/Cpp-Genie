
#include"SearchEngine.h"

class signUp{

private:
    std::string username;
    std::string password;

public:

signUp(){  //default constructor
    username="invalid";
    password="";
}

signUp(std::string username,std::string password):username(std::move(username)),password(std::move(password)){} //parametric constructor


    bool valid()
    {
        int spc=0,nc=0,ac=0;

        for(int i =0;i<password.length();i++)
        {
            if((password[i]<=47 && password[i]>=33) || (password[i]<=64 && password[i]>=58))
            {
                spc++;
            }
            if(password[i]<=57 && password[i]>=48)
            {
                nc++;
            }
            if((password[i]<=90 && password[i]>=65) || (password[i]<=122 && password[i]>=97))
            {
                ac++;
            }

        }
        if(spc>=1 && nc>=1 && ac>=1 && password.length()>=8)
            {
                return true;
            }
        return false;
    }
    void signup()
    {
        if(username!="" && password!="")
        {
            string dirPath = createDirectory();
            string path = dirPath + string("\\\\password.txt");

            ofstream file;
            //create a file
            file.open(path.c_str());
            //file name is username.txt
            file << username << endl <<password;
            //file holds username is first line and password in the other
            file.close();
        }
    }

    string createDirectory()
    {
        DirectoryHandler d1;
        std::string dirpath = std::string(".\\\\")+username;  //D:\\\\C++GenieFile\\\\") + username;
        d1.setDirName(dirpath.data());

        if(d1.exists())
        {
            std::cout<<"\ndirectory already exists";
        }
        else
        {
            d1.createDir();
            d1.hidedir();
        }

        return dirpath;
    }
    void createTodoDirectory()
    {
        DirectoryHandler d1;
        std::string dirpath = std::string(".\\\\")+username+std::string("\\\\Todo");  //D:\\\\C++GenieFile\\\\") + username;
        d1.setDirName(dirpath.data());
        if(d1.exists())
        { std::cout<<"\nTodo directory already exists"; }
        else
        { d1.createDir(); }

        dirpath = std::string(".\\\\")+username+std::string("\\\\data");  //D:\\\\C++GenieFile\\\\") + username;
        d1.setDirName(dirpath.data());
        if(d1.exists())
        { std::cout<<"\nSearch directory already exists"; }
        else
        { d1.createDir(); }

    }

    bool userExists()
    {
        DirectoryHandler checkuserDhl;
        checkuserDhl.setDirName("./");
        checkuserDhl.setDirForUse();
        checkuserDhl.displayfile();
    for(int i=0;i<checkuserDhl.dirDi.getsize();i++)
    {
        if(checkuserDhl.dirDi.getinfo(i) == username)
            return true;
    }
    return false;

    }
};
