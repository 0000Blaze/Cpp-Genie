#include <GL/gl.h>      //lib for openGL
#include <GL/glu.h>     //lib of glut utilities
#include <GL/glut.h>    //lib glut


//input to be taken from user input on glut screen


#include<iostream>
#include<string>
#include<fstream>
#include<dirent.h>

#include "functions.h"
#include "signUp.h"

using namespace std;

class ToDo{

private:
    std::string description;
    std::string dueDate;
    std::string username;
    std::string reader1="";
    std::string reader2="";
    std::string path;

public:
 ToDo(){  //default constructor
        description="None";
        dueDate="none";
    }
ToDo(string description,string dueDate,string username):description(std::move(description)),dueDate(std::move(dueDate)),username(std::move(username)){} //parameter constructor

void addTodo(){
        string dirPath = createDirectory1();
        path = dirPath +std::string(description) + std::string(".txt");

        ofstream file; //create a file
        file.open((path).c_str());
        file << description <<std::endl<<dueDate<<std::endl;
        file.close();
}

void readTodo(){
    unsigned int counter =1;

    ifstream fin;
    fin.open((path).c_str(),ios::app);  //open ToDo text file

    if(!fin)      //exception handling if file not open
        {
        std::cout<<"unable to open";
        }

    else  //file open work
    {

        do{
                if(counter==1)
                {
                    getline(fin,reader1); //read description from file
                    counter=2;
                }

                else if(counter==2)
                {
                    getline(fin,reader2);
                    counter=3;
                }
                if(counter==3)
                {
                    break;
                }



        }while(!fin.eof());

    fin.close(); //close file
    }

}

string desFromFile ()
{
    return reader1;
}
string dateFromFile()
{
    return reader2;
}

string createDirectory1()
    {
        DirectoryHandler d2;
        std::string dirpath = std::string(".\\\\") + username+std::string("\\\\Todo\\\\");      //D:\\\\C++GenieFile
        d2.setDirName(dirpath.data());

        if(d2.exists())
        {
            //std::cout<<"\ndirectory already exists";
        }
        else
        {
            d2.createDir();
        }

        return dirpath;
    }
};
