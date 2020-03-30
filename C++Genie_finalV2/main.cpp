#include <GL/gl.h>      //lib for openGL
#include <GL/glu.h>     //lib of glut utilities
#include <GL/glut.h>    //lib glut

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include "Headers/Timecl.h"
#include "Headers/TodoList.h"
#include "Headers/vars.h"
#include "Headers/SearchEnginePage.h"
#include "Headers/ErrorWindow.h"

const int  WID = 700;
const int  HEI = 650;
const int  INI_X = 350;
const int  INI_Y = 70;
const int  WC_R = 0.094;
const float WC_G = 0.157;
const float WC_B = 0.4;
const float WC_A = 1.0;
const float COORD_X = 10;
const float COORD_Y = 10;
const int  DEL_KEY = 8;
const int  ENTER_KEY = 13;
const int  ESC_KEY = 27;
const int  TAB_KEY = 9;
const int lx=-0.7;
const int ly=-8;

enum
{
    WELCOME_P =-1,
    LOGIN_P = 0,
    WORKING_P =1,
    TODO_P =2,
    SIGNUP_P =10
};
using namespace std;

void callBackFun(); //initial on the window
void initColor();//color of the window
void ReshapeCallBack(int wid, int heig); // in case of change in window size,avoid distortion
void mouseclicked(int button,int state,int x,int y);//action of mouse click
void keyPressed(unsigned char key,int x, int y); //keyboard
void passMouse(int x,int y);

class windows{
private:
    const int lyt=-4;
    std::string *task;
    int i=0;

public:
    void welcomePage();
    void signUpPage();
    void loginPage();
    void workScreen();
    void todoScreen(); //to do list and reminder window
}navigate;

//features
    void addMenu();
    void activateMenu();
    void deactivateMneu();
    void createErrorWindow(const char*);
    void clockpart();

float X=7.0;
float Y=5.0;
float Xc=2.0;
float Yc=3.0;
float pageNo=-1;
float dx;
float dy;
float todo_x=-8;
float todo_y=-2;
float x_pos =-10.0;

std::string userName="";
std::string password="";
std::string desText="";
std::string dueText="";
std::string userNameN="";
std::string passwordN="";
std::string *files;

int bossel =1;
int enterCount=0;
int dir = 1;
int numFile;
int menuIndex;
int mainWindowIndex;


long long unsigned int blinkera =0;
long long unsigned int blinkerb =0;
long long unsigned int blinkerc =0;

bool showTextUserName =true;
bool showTextPass = false;
bool showTextUserNameN =true;
bool showTextPassN = false;
bool showDes=true;
bool showDueDate=false;
bool validLogIn=false;
bool showTodo =false;
bool showTodoList=true;
bool mouseAtExit =false;
bool menuActive = false;

//GLUT functions

int main(int argc, char ** argv)    //default arguments of main
{

    glutInit(&argc, argv);  //always required to render glut
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //sets the display mode of the window
    glutInitWindowPosition(INI_X ,INI_Y ); //initialize the position of window when 1st displayed in screen
    glutInitWindowSize(WID,HEI); //width and height of windows in pixels
    mainWindowIndex = glutCreateWindow("C++Genie");    //defines the name of the window

    glutDisplayFunc(callBackFun); //takes function pointer and is used to draw on window
    glutReshapeFunc(ReshapeCallBack);//used to register a callback , argument function pointer
    glutMouseFunc(mouseclicked);//mouse click function redirects to different pages
    glutMotionFunc(SearchWindow::OnMouseDrag);  //mouse dragged callback function
    glutPassiveMotionFunc(passMouse);
    glutKeyboardFunc(keyPressed);//keyboard press function

    addMenu();

    initColor(); //background color function
    glutCreateSubWindow(mainWindowIndex,1,1,200,100);
    glutDisplayFunc(clockpart);
    glutReshapeFunc(ReshapeCallBack);
    initColor();

    glutMainLoop();  //glut loop required to keep window live until exit is done
}

void callBackFun()    //Call  back function to draw on the window
{
    glClear(GL_COLOR_BUFFER_BIT); //Clears the frame buffer of window,good habit
    glLoadIdentity(); //resets the matrix transformation done is previous options(i.e.,make default coordinate points)

    if(pageNo==WELCOME_P)
    {
        navigate.welcomePage();
        glutSwapBuffers();
    }
    if(pageNo==SIGNUP_P)
    {
        navigate.signUpPage();
        glutSwapBuffers();
    }
    if(pageNo==LOGIN_P)
    {
        navigate.loginPage();
        glutSwapBuffers();
    }

    else if(pageNo==WORKING_P)
    {
        navigate.workScreen();
        glutSwapBuffers();
    }
    else if(pageNo==TODO_P)
    {
        navigate.todoScreen();
        glutSwapBuffers();
    }
}

void initColor() //Sets initial Background Color of the window
{
    glClearColor(WC_R,WC_G,WC_B,WC_A); // RGB and Alpha  //color intensity of color of screen
    ErrorWindow::mainWindowIndex = mainWindowIndex;   //set window index of main to error window
    SearchWindow::AddFileWindow::mainWindowIndex = mainWindowIndex;
}

void ReshapeCallBack(int wid, int heig)// width and height passed by the API
{
    glViewport(0,0,wid,heig); //full screen has been taken as view port //all drawings are displayed in the view port
    glMatrixMode(GL_PROJECTION); //Switches to projection mode to draw stuff on the window //from GL model view matrix(default)
    glLoadIdentity(); //resets the parameters of projection matrix
    gluOrtho2D(-COORD_X,COORD_X,-COORD_Y,COORD_Y); //takes 4 arguments left,right,bottom,top (2D graph)
    glMatrixMode(GL_MODELVIEW); //switches back to model-view mode,the default mode
    //always should be in model view matrix , only go to projection matrix to set coordinates
}

void createErrorWindow(const char* err)
{
     if(ErrorWindow::canMake)
            ErrorWindow::create(err,glutGet(GLUT_WINDOW_X)+150,glutGet(GLUT_WINDOW_Y)+200);
}
namespace action
{
    void loginButtonPressed(logIn login)
    {
        if(userName=="")
        {
            createErrorWindow("Please enter user-name");
        }
        else if(login.IsLogedIn())
        {
            pageNo=WORKING_P;               //to working page
            SearchWindow::setUser(userName);
            SearchWindow::createFileBox();
        }
        else
        {
            createErrorWindow("The User-name or Password doesn't match");
        }
    }
    void signUpButton(signUp signup)
    {
        if(userNameN!="" && passwordN!="")
        {
            if(!signup.userExists())
            {
                if(signup.valid())
                {
                    signup.signup();   //create sign up file
                    signup.createTodoDirectory();
                    pageNo=WORKING_P;
                    userName=userNameN;
                    SearchWindow::setUser(userName);
                    SearchWindow::createFileBox();
                }
                else
                    createErrorWindow("Password must contain special character, alphabet & digit");
            }
            else
            {
                    createErrorWindow("User already exists");
            }
        }
        else if(userNameN!="" || passwordN!="")
        {
            createErrorWindow("User-name and password required!");
        }
    }
}
namespace WelcomePage
{
    void mouse(int x, int y)
    {
        //std::cout<<y;
        if((x>=365 && x<=490) && (y>=570 && y<=615))
        {
            pageNo=LOGIN_P;
        }
        if((x>=505 && x<=630) && (y>=570 && y<=615))
        {
            pageNo=SIGNUP_P;
        }
    }
}
namespace logInN
{
    void KeyBoard(unsigned char pressedKey,logIn l)
    {
        if(pressedKey>=36 && pressedKey <=126)
        {
            if(bossel==1)
            {
                userName+=pressedKey;
                showTextUserName=true;
                enterCount=1;
            }
            else if(bossel==2)
            {
                password+=pressedKey;
                showTextPass=true;
                enterCount=2;
            }
        }
        else if((pressedKey==TAB_KEY || pressedKey==ENTER_KEY) && enterCount==1)
        {
            showTextPass=true;
            bossel=2;
            enterCount=2;
        }
        else if(pressedKey==8)
        {
            if(showTextUserName && bossel==1)
            {
                if(userName!="")
                {
                    userName.pop_back();
                }

            }
            if(showTextPass && bossel==2)
            {
                if(password!="")
                password.pop_back();
            }
        }
        else if(pressedKey==ENTER_KEY && enterCount==2)
        {
            action::loginButtonPressed(l);
        }
    }
    void mouse(int x,int y,logIn login)
    {
        if((x<=680 && x>=635) && (y>=15 && y<=40)) //EXIT BUTTON
         {
             ExitProcess(0);
         }
        if((x>=300 && x<=450) && (y>=440 && y<=495)) //login page to working page
        {
            action::loginButtonPressed(login);
        }
        else if((x<=560 && x>=140) && (y<=275 && y>=210))
        {
            showTextUserName=true;
            showTextPass=false;
            bossel=1;
            enterCount=1;
        }
        else if((x<=560 && x>=140) && (y<=405 && y>=340))
        {
            showTextPass=true;
            showTextUserName=false;
            bossel=2;
            enterCount=2;
        }
        else if((x<=665 && x>=505) && (y<=635 && y>=595))
        {
            pageNo=SIGNUP_P;
            bossel=5;
            showTextUserNameN=true;
            showTextPassN=false;
            userNameN="";
            passwordN="";
        }
    }
}

namespace WorkScreen
{
    void KeyBoard(unsigned char pressedKey,int x,int y)
    {
        SearchWindow::OnKeyPressed(pressedKey,x,y);
    }
    void mouse(int button,int state,int x,int y)
    {
        SearchWindow::mouseFunc(button,state, x, y);
        if((x<=680 && x>=635) && (y>=15 && y<=40)) //Log Out
        {
            pageNo=LOGIN_P;
            userName="";
            password="";
            showTextUserName=true;
            showTextPass=false;
            bossel=1;
            enterCount = 0;
            }
        else if((x>=580 && x<=670) && (y>=65 && y<=100)) //from working page to to do page
        {
            bossel=3;
            pageNo=TODO_P;
        }
    }
}
namespace Todo
{
    void KeyBoard(unsigned char pressedKey)
    {
        if(bossel==2 || bossel ==6)
            bossel==3;
        if((pressedKey>=36 && pressedKey <=126) || pressedKey==32)
        {
            if(bossel==3)
            {
                desText+=pressedKey;
                showDes=true;
                enterCount=3;
            }
            else if(bossel==4)
            {
                dueText+=pressedKey;
                showDueDate=true;
                enterCount=4;
            }
        }
        else if (pressedKey == ENTER_KEY || pressedKey == TAB_KEY)
        {
            if(bossel==3)
            {
                showDueDate=true;
                bossel=4;
            }
            else if(pressedKey==ENTER_KEY && bossel==4)
            {
                showTodo=true;
                showDes=true;
            }
        }
        else if(pressedKey==8)
        {
            if(showDes && bossel==3)
            {
                if(desText!="")
                desText.pop_back();
            }
            else if(showDueDate && bossel==4)
            {
                if(dueText!="")
                dueText.pop_back();
            }
        }
    }
    void mouse(int x,int y)
    {
        dx=toFloatX(x);
        dy=toFloatY(y);
        for(int i=0; i<numFile ;i++)
        {
            if((dx>todo_x+10 && dx<todo_x+12.4) && (dy<todo_y-2.5-(static_cast<float>(i)/2) && dy>todo_y-(static_cast<float>(i)/2)-2.9))
            {
                //
                std::cout<<files[i];
                std::remove((userName+"\\\\Todo\\\\"+files[i]+".txt").c_str());
            }
        }
        if((x<=525 && x>=245) && (y<=180 && y>=140))
        {
            bossel=3;
            showDes=true;
            showDueDate=false;
            enterCount=3;
        }
        if((x<=525 && x>=245) && (y<=245 && y>=207))
        {
            bossel=4;
            showDueDate=true;
            enterCount=4;
        }
        else if((x<=420 && x>=315) && (y<=290 && y>=260))    //Add Button
        {
            showTodo=true;
        }
        else if((x<=680 || x>=635) && (y>=15 && y<=40)) //Back Button
        {
            pageNo=WORKING_P;
            desText="";
            dueText="";
            showDes=true;
            showDueDate=false;
            bossel=3;
        }
    }
}
namespace signUpN
{
    void KeyBoard(unsigned char pressedKey, signUp signup)
    {
        if(bossel==1 || bossel==3)
           bossel=5;
        if(pressedKey>=36 && pressedKey <=126)
        {
            if(bossel==5)
            {
                enterCount=5;
                userNameN+=pressedKey;
                showTextUserNameN=true;
            }

            else if(bossel==6)
            {
                enterCount=6;
                passwordN+=pressedKey;
                showTextPassN=true;
            }
        }
        else if(pressedKey==8)
        {
            if(showTextUserNameN && bossel==5)
            {
                if(userNameN!="")
                    userNameN.pop_back();
            }
            else if(showTextPassN && bossel==6)
            {
                if(passwordN!="")
                    passwordN.pop_back();
            }
        }
        else if ((pressedKey == ENTER_KEY || pressedKey == TAB_KEY) && enterCount!=6)
        {
            enterCount=6;
            showTextPassN=true;
            bossel=6;
        }
        else if(pressedKey==ENTER_KEY && enterCount==6)
        {
            action::signUpButton(signup);
        }
    }
    void mouse(int x,int y,signUp signup)
    {
        if((x>=300 && x<=450) && (y>=450 && y<=495)) //sign up page to working page
        {
            action::signUpButton(signup);
        }
        else if((x<=560 && x>=140) && (y<=275 && y>=210))
        {
            bossel=5;
            showTextUserNameN=true;
            showTextPassN=false;
        }
        else if((x<=560 && x>=140) && (y<=405 && y>=340))
        {
            bossel=6;
            showTextPassN=true;
        }
        else if((x<=448 && x>=385) && (y<=600 && y>=565))   //redirects to login page
        {
            userName="";
            password="";
            showTextUserName=true;
            showTextPass=false;
            bossel=1;
            pageNo=LOGIN_P;
        }
    }
}

void keyPressed(unsigned char pressedKey,int x,int y)
{
    if(pageNo==LOGIN_P)
    {
        logIn logine(userName,password);
        logInN::KeyBoard(pressedKey,logine);
    }

    else if(pageNo==WORKING_P)
    {
        WorkScreen::KeyBoard(pressedKey,x,y);
    }
    else if(pageNo==TODO_P)
    {
        Todo::KeyBoard(pressedKey);
    }
    else if(pageNo==SIGNUP_P)
    {
        signUp sign(userNameN,passwordN);
        signUpN::KeyBoard(pressedKey,sign);
    }
    glutPostRedisplay();
}

void mouseclicked(int button,int state,int a,int b)
{
    int MAX_X=glutGet(GLUT_WINDOW_WIDTH);
    int MAX_Y=glutGet(GLUT_WINDOW_HEIGHT);
    int x=(a/static_cast<float>(MAX_X))*700;
    int y=(b/static_cast<float>(MAX_Y))*650;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(pageNo==WELCOME_P)          //Welcome Page
        {
            WelcomePage::mouse(x,y);
        }
        else if(pageNo==LOGIN_P)       //login page
        {
            logIn log(userName,password);
            logInN::mouse(x,y,log);
        }
        else if (pageNo==SIGNUP_P)
        {
            signUp sup(userNameN,passwordN);
            signUpN::mouse(x,y,sup);
        }
        else if(pageNo==WORKING_P)
        {
            WorkScreen::mouse(button,state,x,y);
        }
        else if(pageNo==TODO_P)
        {
            Todo::mouse(x,y);
        }
    }
    else if((button == 3 || button == 4)&& pageNo==WORKING_P)
    {
        SearchWindow::mouseFunc(button,state, x, y);
    }
    glutPostRedisplay();
}
void passMouse(int x, int y)
{
    if((x<=680 && x>=615) && (y>=15 && y<=40) && pageNo==LOGIN_P) //EXIT BUTTON
    {
        glutSetCursor(GLUT_CURSOR_HELP);
        mouseAtExit=true;
    }
    else
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        mouseAtExit=false;
    }

    if(SearchWindow::mouseOverScrollBox(x,y)&& pageNo == WORKING_P)
    {
        activateMenu();
        SearchWindow::OnMouseHover(x,y);
    }
    else
    {
        deactivateMneu();
        SearchWindow::resetActivePos();
    }
}
//class windows functions

void windows::welcomePage()
{
    glColor3f(1,1,0);
    glDrawP(lx+.5,ly+.5,3.5,1.5);
    glDrawP(lx+4.5,ly+.5,4,1.5);
    printText(-2.1,3,"C++ Genie",GLUT_BITMAP_TIMES_ROMAN_24,1,1,0);
    printText(-2.9,2.2,"Local Storage System",GLUT_BITMAP_HELVETICA_18,1,1,0);
    printText(lx-7.3,lyt+1,"DEVELOPED BY:",GLUT_BITMAP_HELVETICA_18,1,1,1);
    printText(lx-7.3,lyt+.1,"Ravi Pandey",GLUT_BITMAP_HELVETICA_18,1,1,1);
    printText(lx-7.3,lyt-.85,"Rohan Chhetry",GLUT_BITMAP_HELVETICA_18,1,1,1);
    printText(lx-7.3,lyt-1.9,"Nikesh D.C.",GLUT_BITMAP_HELVETICA_18,1,1,1);
    printText(lx+1.5,ly-0.35,"LOG IN",GLUT_BITMAP_8_BY_13,0,0,0);
    printText(lx+5.32,ly-0.35,"SIGN UP",GLUT_BITMAP_8_BY_13,0,0,0);
}

void windows::signUpPage()
{
    glColor3f(0.78,0.749,0.9055);
    glDrawP(1,-7.4,1.8,1);  //Back to login page button
    printText(-3.1,-8,"Already a user? :",GLUT_BITMAP_HELVETICA_18,0.5,1,0.5);
    printText(1.2,-8,"Login",GLUT_BITMAP_HELVETICA_18,0,0,0);
    glDrawRecOutline(-X-.1,Y+.1,X+.1,-Y+1.9);
    glDrawRecOutline(-X,Y,X,-Y+2);
    printText(-X+5,Y+0.51,"Sign-up Screen",GLUT_BITMAP_TIMES_ROMAN_24,1.0,0,0);
    printText(-X+1,Y-1,"User-name",GLUT_BITMAP_8_BY_13,1,1,0);
    printText(-X+1,-Y+5,"Password",GLUT_BITMAP_8_BY_13,1,1,0);
    glColor3f(0.78,0.749,0.9055);
    glDrawP(-X+1,Y-1.5,(2*X)-2,2);
    glDrawP(-X+1,Y-5.5,(2*X)-2,2);
    glDrawP(-1.5,-3.5,4,1.5);
     if(showTextUserNameN)// || showTextPass)
     {
        printText(-X+1.2,-Y+7.2,userNameN.c_str(),GLUT_BITMAP_9_BY_15,0,0,0);
        if(!showTextPassN && showTextUserNameN)
        {
        blinkerc++;
        if (blinkerc%200>=0 && blinkerc%200<=100)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'|');
        glutPostRedisplay();
        }
    }
    if(showTextPassN)
    {
        printText(-X+1.2,-Y+3.2,"",GLUT_BITMAP_9_BY_15,0,0,0);
        for( unsigned int l=0;l<passwordN.length();l++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'*');
        if(showTextPassN)
        {
            blinkera=0;
            blinkerb=0;
            blinkerc++;
            if (blinkerc%200>=0 && blinkerc%200<=100)
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'|');
        glutPostRedisplay();
        }
    }
    printText(-0.5,-4.3,"Sign Up",GLUT_BITMAP_8_BY_13,0,0,0);
    glColor3f(0,0,0);
    glDrawRecOutline(X-12.9,Y-3.4,X-1.1,Y-1.6);
    glDrawRecOutline(X-12.9,Y-7.4,X-1.1,Y-5.6);
    glColor3f(1,1,0);
}

void windows::loginPage()
{
    if(mouseAtExit)
    {
        glColor3f(0.9,0.1,0.1);       //EXIT
    }
    else
    {
        glColor3f(WC_R,WC_G,WC_B);
        mouseAtExit=false;
    }
    glDrawP(9.5,8.7,-2.1,-.8);
    printText(7.5,8.9,"  EXIT",GLUT_BITMAP_HELVETICA_18,1,1,1);
    printText(-X+5.5,Y+0.51,"LOG IN",GLUT_BITMAP_TIMES_ROMAN_24,1.0,0.7,0);
    printText(-X+1,Y-1,"User-name",GLUT_BITMAP_8_BY_13,1,1,0);
    printText(-X+1,-Y+5,"Password",GLUT_BITMAP_8_BY_13,1,1,0);
    glColor3f(0.78,0.749,0.9055);
    glDrawP(-X+1,Y-1.5,(2*X)-2,2);
    glDrawP(-X+1,Y-5.5,(2*X)-2,2);
    glDrawP(-1.5,-3.5,4,1.5);
    if(showTextUserName || bossel==1 || showTextPass)
    {
        printText(userName,-X+1.2,-Y+7.2,6,GLUT_BITMAP_9_BY_15);
        if(!showTextPass)
        {
            blinkera++;
            if (blinkera%200>=0 && blinkera%200<=100)
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'|');
            glutPostRedisplay();
        }
    }
    if(showTextPass)
    {
        printText(-X+1.2,-Y+3.2,"",GLUT_BITMAP_9_BY_15,0,0,0);
        for(unsigned int l=0;l<password.length();l++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'*');
        }
        if(showTextPass)
        {
            blinkerb=0;
            blinkerc=0;
            blinkera++;
            if (blinkera%200>=0 && blinkera%200<=100)
            {
                if(password.length()>=33)
                {
                    glColor3f(0,0,0);
                    glRasterPos2f(5.58,-1.8);
                    glColor3f(1,1,0);
                }
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'|');
            }
            glutPostRedisplay();
        }
        glColor3f(0.78,0.749,0.9055);
        glDrawP(5.67,-2,.33,-1);
        glColor3f(WC_R,WC_G,WC_B);
        glDrawP(6,-2,4,-1);
    }
    printText(-0.5,-4.3,"LOG IN",GLUT_BITMAP_8_BY_13,0,0,0);
    glDrawP(4.5,-9.5,4.5,-1.2);
    printText(4.9,-8.9,"Don't have an Account?",GLUT_BITMAP_HELVETICA_10,0,0,0);
    glColor3f(1,1,0);
    glDrawRecOutline(-X-.1,Y+.1,X+.1,-Y+1.9);
    glDrawRecOutline(-X,Y,X,-Y+2);
    glColor3f(0,0,0);
    glDrawRecOutline(X-12.9,Y-3.4,X-1.1,Y-1.6);
    glDrawRecOutline(X-12.9,Y-7.4,X-1.1,Y-5.6);
    glColor3f(1,1,0);
}

void windows::workScreen()
{
    glRasterPos2f(-2,9);
    printText(-2.25,8.8,("Hi,"+userName).c_str(),GLUT_BITMAP_HELVETICA_18,1,1,0);
    glColor3f(1,1,1);
    glDrawP(9.5,8.7,-2.1,-0.8);
    printText(7.5,8.9,"Log Out",GLUT_BITMAP_HELVETICA_18,0,0,0);
    glColor3f(0.78,0.749,0.9055); //color of todolist button
    glDrawP(9.5,7,-3,-1);
    printText(6.9,7.3,"ToDo list",GLUT_BITMAP_8_BY_13,0,0,0);
    SearchWindow::makeObjects();
}

void windows::todoScreen()
{
    glRasterPos2f(-2,9);
    printText(-2.25,8.8,("Hi,"+userName).c_str(),GLUT_BITMAP_HELVETICA_18,1,1,0);
    glColor3f(1,1,1);
    glDrawP(9.5,8.7,-1.3,-0.8); //BACK BUTTON
    printText(8.3,8.9,"Back",GLUT_BITMAP_HELVETICA_18,0,0,0);
    glColor3f(0.78,0.749,0.9055); //color of glut polygon
    glDrawP(-Xc+7,Yc+2.6,-8,1.1); //description box
    glDrawP(-Xc+7,Yc+0.6,-8,1.1); //due date box
    printText(-Xc-1.3,Yc+3.5,"Add to list:",GLUT_BITMAP_TIMES_ROMAN_24,0.5,0.5,1);
    printText(-Xc-1.1,Yc+2.8,"Title:",GLUT_BITMAP_8_BY_13,1,1,0);
    printText(-Xc-1.1,Yc+1,"Due Date(DD/MM/YY):",GLUT_BITMAP_8_BY_13,1,1,0);
    if(showDes || showDueDate)
    {
        //keyboard input description
        printText(desText,-Xc-0.85,Yc+1.8,-Xc+8,GLUT_BITMAP_8_BY_13);
        if(showDes && !showDueDate)
        {
            blinkera=0;
            blinkerc=0;
            blinkerb++;
            if (blinkerb%200>=0 && blinkerb%200<=100)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,'|');
            glutPostRedisplay();
        }
    }
    if(showDueDate || showDes)
    {
        //keyboard input due date
        printText(dueText,-Xc-0.85,Yc-0.2,-Xc+8,GLUT_BITMAP_8_BY_13);
        if(showDueDate)
        {
            blinkerb++;
            if (blinkerb%200>=0 && blinkerb%200<=100)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,'|');
                //glutStrokeCharacter(GLUT_STROKE_ROMAN,'|');
            glutPostRedisplay();
        }
    }
    glColor3f(0.8,0.5,0.7); //add button color
    glDrawP(-1,1,3,-1); //Add Butto
    printText(-0.8,1.4,"Add to list",GLUT_BITMAP_8_BY_13,0,0,0); //add button
    ToDo todo(desText,dueText,userName);
    DirectoryHandler todoNum;
    //use of *file,numFile,i
    if(showTodo)
    {
            todo.addTodo(); //std::cout<<"\nfile made\n";
            todo.readTodo();
            showTodo=false;
            showTodoList=true;
    }
    todoNum.setDirName(std::string("./"+userName+"/Todo/").c_str());
    todoNum.setFileNames();
    numFile=todoNum.getFileNumber();
    files = new string[numFile];
    task = new string[numFile];
    for(i=0;i<numFile;i++)
        {
            files[i]=todoNum.getFileName(i);
            task[i]=viewTodoList(userName,files[i]);
        }
    if(showTodoList)
    {
        for(i=0;i<numFile;i++)
        {
            if(files[i]!="")
            {
                printText(todo_x-1,todo_y-1,"TODO LIST",GLUT_BITMAP_TIMES_ROMAN_24,1,1,1);
                printText(todo_x-0.5,todo_y-2,"Tasks\t\t\t\t",GLUT_BITMAP_HELVETICA_18,1,1,1);
                printText(todo_x+5.5,todo_y-2,"Due Date",GLUT_BITMAP_HELVETICA_18,1,1,1);
                for(int popper=0;popper<4;popper++)
                    files[i].pop_back();

                glColor3f(1,0.1,0.1);

                for(int d=0;d<numFile;d++)
                {
                    glDrawP(todo_x+10,todo_y-2.3-static_cast<float>(i)/2,2.4,0.4);
                }
                printText(todo_x-0.5,todo_y-2.5-(0.5*i),(files[i]).c_str(),GLUT_BITMAP_8_BY_13,1,1,1);
                printText(todo_x+5.5,todo_y-2.5-(0.5*i),(task[i]).c_str(),GLUT_BITMAP_8_BY_13,1,1,1);
                printText(todo_x+10.15,todo_y-2.6-(0.5*i)," Delete",GLUT_BITMAP_8_BY_13,1,1,1);
            }
        }
    }
}
//feature functions
void addMenu()
{
         menuIndex = glutCreateMenu(SearchWindow::menuCallback);
        // Add menu items
        glutAddMenuEntry("Create Text File", CREATE_FILE);
        glutAddMenuEntry("Edit File", EDIT_FILE);
        glutAddMenuEntry("Delete File", DELETE_FILE);
        glutAddMenuEntry("Add Existing File",ADD_FILE);
        glutAddMenuEntry("Rename File",RENAME_FILE);

}
void activateMenu()
{
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void deactivateMneu()
{
    glutDetachMenu(GLUT_RIGHT_BUTTON);
}

void clockpart()
{
    glClear(GL_COLOR_BUFFER_BIT); //Clears the frame buffer of window,good habit

    glLoadIdentity(); //resets the matrix transformation done is previous options(i.e.,make default coordinate points)
    glClearColor(WC_R,WC_G,WC_B,WC_A);

    Showtime s1(-9,1.5,1,1,1);
    s1.displayClock(GLUT_BITMAP_TIMES_ROMAN_24);
    s1.displayCalendar(-5,-2.5,GLUT_BITMAP_HELVETICA_18);
    glutPostRedisplay();
    glutSwapBuffers();
}
