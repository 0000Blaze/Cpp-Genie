#include"login.h"
#include "ScrollBox.h"

const float SB_X1 = 0.2*10;  //search box
const float SB_Y1 = 0.6*10;
const float SB_X2 = -0.8*10;
const float SB_Y2 = 0.5*10;

const float SYNB1_X1 = 0.9*10;  //synonym box 1
const float SYNB1_Y1 = 0.25*10;
const float SYNB1_X2 = 0.5*10;
const float SYNB1_Y2 = 0.15*10;

const float SYNB2_X1 = SYNB1_X1; //synonym box 1
const float SYNB2_Y1 = SYNB1_Y1-0.2*10;
const float SYNB2_X2 = SYNB1_X2;
const float SYNB2_Y2 = SYNB1_Y2-0.2*10;

const float SSYNB_X1 = SYNB2_X1-1.25;   //set synonym box
const float SSYNB_Y1 = SYNB2_Y2-1;
const float SSYNB_X2 = SSYNB_X1-1.5;
const float SSYNB_Y2 = SSYNB_Y1-1;

const float SCBX_Y2 = -0.1*10;
enum{ ADD_FILE, EDIT_FILE, DELETE_FILE ,CREATE_FILE,RENAME_FILE};

namespace SearchWindow
{
    ScrollBox scbx(-9,SB_Y2-1,SYNB1_X2-1,SCBX_Y2);   //files scroll box
    ScrollBox fbscbx(-9,SCBX_Y2-1,SSYNB_X2-1,SCBX_Y2-8);    //feedback scroll box
    float scrollthreshold = 0.1*10;

    bool isSearching = false;
    bool syn1 = false;
    bool syn2 = false;
    bool wheelSelected = false;      //files scroll box wheel selected
    bool fbwheelSelected = false;     //feedback scroll wheel selected
    bool addingfile = false;
    bool renamingfile = false;

    std::string keyword ("");
    std::string synonym1("");
    std::string synonym2("");
    std::string userName;

    void createFileBox();
    void createFile();
    void addFile();

 namespace AddFileWindow
 {
    int index;
    bool canMake = true;
    int mainWindowIndex;
    string filename("");
    const float CHAR_WIDTH = 0.04;
    const float CHAR_HEIGHT = 0.13;
    const float X1 = -0.5;
    const float X2 = X1+1;
    const float Y1 = 0.6;
    const float Y2 = Y1-0.3;
    TextBox filebox(X1,X2,Y1,Y2,CHAR_WIDTH,CHAR_HEIGHT);
    TextBox dirbox(X1,X2,Y2-0.2,Y2-0.5,CHAR_WIDTH,CHAR_HEIGHT);

    void destroy()
    {
        glutDestroyWindow(index);
        filebox.setActive(false);
        dirbox.setActive(false);
        canMake = true;
        addingfile = false;
        renamingfile = false;
        glutSetWindow(mainWindowIndex);
    }

    void printText(std::string text,float x,float y,float max_x,void* font)
    {
      glColor3f(0,0,0);
      glRasterPos2d(x,y);
      int i = 0;
      float tsize = text.size()* CHAR_WIDTH;
      if( tsize < (max_x-x))
        i =0;
      else
        i = (text.size() - (unsigned int)((max_x - x)/CHAR_WIDTH));
      for (i;i<text.size();i++)
      {
        glutBitmapCharacter(font,text[i]);
      }
      glutPostRedisplay();
    }

    void render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        if(addingfile)
        {
            printText("ENTER FILE-NAME OF SOURCE FILE(eg. file.jpg)",-0.9,Y1+0.1,0.9,GLUT_BITMAP_8_BY_13);
            printText("ENTER DIRECTORY-PATH OF FILE ABOVE(eg. c:/dir1/dir2)",-0.95,Y2-0.15,0.95,GLUT_BITMAP_8_BY_13);
            dirbox.display();
        }
        else if(renamingfile)
        {
            printText("ENTER NAME TO RENAME THE FILE with correct extension",-0.95,Y1+0.1,0.95,GLUT_BITMAP_8_BY_13);
        }
        else
             printText("ENTER FILE-NAME TO CREATE TEXT FILE(eg. new)",-0.9,Y1+0.1,0.9,GLUT_BITMAP_8_BY_13);
        filebox.display();

        glutSwapBuffers();
    }

    void OnKeyPressed(unsigned char key,int x,int y)
    {
      if(addingfile)
        dirbox.onKeyPressed(key);
      filebox.onKeyPressed(key);
      if(filebox.getActive() && key == 13 && !addingfile)
      {
          if(!renamingfile)
          {
            SearchWindow::createFile();
          }
          else
          {
                string newpath = string("./") + userName + string("/data/") + filebox.getText();
                string oldpatn = string("./") + userName + string("/data/") + scbx.getActiveLine();
                rename(oldpatn.data(),newpath.data());
                filebox.setText("");
                dirbox.setText("");
                createFileBox();
          }
            destroy();
      }
      if(dirbox.getActive() && key == 13 && filebox.getText().size()>4)
      {
          string newpath = string("./") + userName + string("/data/") + filebox.getText();
          string oldpatn = dirbox.getText() + "/" + filebox.getText();
          rename(oldpatn.data(),newpath.data());
          filebox.setText("");
          dirbox.setText("");
          createFileBox();
          destroy();
      }
    }

   void mouseFunc(int button,int state,int x,int y)
    {
        if((button == GLUT_LEFT_BUTTON) &&(state == GLUT_DOWN))
        {
            float xf ,yf;
            xf = static_cast<float>(x)/200 - 1;
            yf = 1 - static_cast<float>(y)/100;

             if(addingfile)
                dirbox.setActive(dirbox.mouseFunc(xf,yf));
             filebox.setActive(filebox.mouseFunc(xf,yf));
        }
    }

    void create(unsigned int posx,unsigned int posy)
    {
        if(canMake)
        {
        index = glutCreateWindow("Add File");
        glutSetWindow(index);
        glutPositionWindow(posx,posy);
        glutReshapeWindow(400,200);
        glutDisplayFunc(render);
        glClearColor(0.7, 0.7, 0.7, 1);
        glutMouseFunc(mouseFunc);
        glutKeyboardFunc(OnKeyPressed);
        canMake = false;
        }
    }
}

    void createFile()
    {
        FileHandler cfile;
        string cfilepath = string("./") + userName + string("/data/") + AddFileWindow::filebox.getText() + string(".txt");
        cfile.setFilePath(cfilepath.data());
        cfile.createFile();
        createFileBox();
        AddFileWindow::filebox.setText("");
    }

    void menuCallback(int item)
    {
        FileHandler cfile;
        string cfilepath = string("./") + userName + string("/data/") + scbx.getActiveLine();
        cfile.setFilePath(cfilepath.data());
            switch (item)
            {
            case CREATE_FILE:
            {
            if(AddFileWindow::canMake)
                AddFileWindow::create(glutGet(GLUT_WINDOW_X)+150,glutGet(GLUT_WINDOW_Y)+100);
            break;
            }

            case EDIT_FILE:
            {
                cfile.openFileExternal();
                break;
            }
            case DELETE_FILE:
            {
                cfile.removeFile();
                createFileBox();
                break;
            }
            case ADD_FILE:
            {
               addingfile = true;
            if(AddFileWindow::canMake)
                AddFileWindow::create(glutGet(GLUT_WINDOW_X)+150,glutGet(GLUT_WINDOW_Y)+100);
            break;
            }
            case RENAME_FILE:
            {
                   renamingfile = true;
                if(AddFileWindow::canMake)
                    AddFileWindow::create(glutGet(GLUT_WINDOW_X)+150,glutGet(GLUT_WINDOW_Y)+100);
                break;
            }
            default:
                { break;     }

            }
    }
    void printText(std::string text,float x,float y,float max_x)
    {
      glColor3f(0,0,0);
      glRasterPos2d(x,y);
      int i = 0;
      float tsize = text.size()* CHAR_WIDTH;
      if( tsize < (max_x-x))
        i =0;
      else
        i = (text.size() - (unsigned int)((max_x - x)/CHAR_WIDTH));
      for (i;i<text.size();i++)
      {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13,text[i]);
      }
      glutPostRedisplay();
    }

    void printText(std::string text,float x,float y,float max_x,Color col,void* font)
    {
      glColor3f(col.r,col.g,col.b);
      glRasterPos2d(x,y);
      int i = 0;
      float tsize = text.size()* CHAR_WIDTH;
      if( tsize < (max_x-x))
        i =0;
      else
        i = (text.size() - (unsigned int)((max_x - x)/CHAR_WIDTH));
      for (i;i<text.size();i++)
      {
        glutBitmapCharacter(font,text[i]);
      }
      glutPostRedisplay();
    }

    void makeBox(float X1,float X2,float Y1,float Y2,Color c)
    {
        glBegin(GL_QUADS);
        glColor3f(c.r,c.g,c.b);
        glVertex2f(X2,Y1);    //1st quadrant
        glVertex2f(X1,Y1);    //2nd quadrant , x1 y1 is left-top corner
        glVertex2f(X1,Y2);    //3rd quadrant
        glVertex2f(X2,Y2);    //4th quadrant , x2 y2 is right-bottom corner
        glEnd();
    }

    void makeObjects()
    {
        makeBox(SB_X1,SB_X2,SB_Y1,SB_Y2,Color(1,1,1));  //search text box
        makeBox(SB_X1+1,SB_X1+3.5,SB_Y1,SB_Y2,Color(0.8,0.85,0.8));  //search button
        if(isSearching)
            makeBox(SB_X1-0.1,SB_X2+0.1,SB_Y1-0.1,SB_Y2+0.1,Color(0.9,0.9,0.9));  //search box active status box

        makeBox(SYNB1_X1,SYNB1_X2,SYNB1_Y1,SYNB1_Y2,Color(1,1,1));  //SYNONYM BOX 1
        if(syn1)
            makeBox(SYNB1_X1-0.1,SYNB1_X2+0.1,SYNB1_Y1-0.1,SYNB1_Y2+0.1,Color(0.9,0.9,0.9));  //SYNONYM BOX 1

        makeBox(SYNB2_X1,SYNB2_X2,SYNB2_Y1,SYNB2_Y2,Color(1,1,1)); //SYNONYM BOX 2
        if(syn2)
            makeBox(SYNB2_X1-0.1,SYNB2_X2+0.1,SYNB2_Y1-0.1,SYNB2_Y2+0.1,Color(0.9,0.9,0.9)); //SYNONYM BOX 2

        makeBox(SSYNB_X1,SSYNB_X2,SSYNB_Y1,SSYNB_Y2,Color(0.8,0.85,0.8));  //synonym set box
        scbx.display();
        fbscbx.display();

        if(!isSearching && keyword.size() == 0)
             printText(string("enter sentence or word to search for"),SB_X2+0.1,SB_Y2+0.3,100,Color(0.7,0.7,0.7),GLUT_BITMAP_8_BY_13);
        if(!syn1 && synonym1.size() == 0)
            printText(string("word 1"),SYNB1_X2+0.1,SYNB1_Y2+0.3,100,Color(0.6,0.6,0.6),GLUT_BITMAP_8_BY_13);
        if(!syn2 && synonym2.size() == 0)
            printText(string("word 2"),SYNB2_X2+0.1,SYNB2_Y2+0.3,100,Color(0.5,0.5,0.5),GLUT_BITMAP_8_BY_13);

        printText(string("SEARCH"),SB_X1+1.3,SB_Y2+0.35,SB_X1+4.7); //text for search button
        printText(string("RESULTS"),fbscbx.getx1(),fbscbx.gety1()+0.3,fbscbx.getx2(),Color(0.9686,0.9098,0.2),GLUT_BITMAP_8_BY_13);
        printText(string("FILES"),scbx.getx1(),scbx.gety1()+0.3,scbx.getx2(),Color(0.9686,0.9098,0.2),GLUT_BITMAP_8_BY_13);
        printText(string("ADD"),SSYNB_X2+0.4,SSYNB_Y2+0.35,SSYNB_X1); //text for synonym set
        printText(string("ADD SYNONYMS TO ENHANCE"),SYNB1_X2-0.5,SYNB1_Y1+1,100,Color(0.9686,0.9098,0.2),GLUT_BITMAP_8_BY_13); //text for synonym set
        printText(string("FURTHER SEARCHES"),SYNB1_X2-0.5,SYNB1_Y1+0.5,100,Color(0.9686,0.9098,0.2),GLUT_BITMAP_8_BY_13); //text for synonym set

        printText(keyword,SB_X2+0.2,SB_Y2+0.3,SB_X1-0.2);
        printText(synonym1,SYNB1_X2+0.02*10,SYNB1_Y2+0.03*10,SYNB1_X1);
        printText(synonym2,SYNB2_X2+0.02*10,SYNB2_Y2+0.03*10,SYNB2_X1);
    }

    void searchKeyword()
    {
        if(keyword.size()>1)
        {
        string searchpath = string("./") + userName + string("/data/");
        Search(keyword.data(),searchpath.data());
        keyword = "";
        fbscbx.setlines(searchEngineDi.getinfo(),searchEngineDi.getsize());
        searchEngineDi.clear();
        }
    }

    void setSyn()
    {
        if (synonym1.size() > 2 && synonym2.size()>2)
        {
         SetSynonym(synonym1.data(),synonym2.data());
         synonym1 = "";
         synonym2 = "";
        }
    }

    void setToString(string& setstr, unsigned int key)
    {
        if((key == 8)&&(setstr!=""))
        {
            setstr.pop_back();
        }
        else if((key == 13 ) && (setstr.size()>1))
            {
                if(isSearching)
                {
                    searchKeyword();
                }
                else if(syn1)
                {
                    syn2 = true;
                }
                else if(syn2)
                {
                     setSyn();
                }
                else{  syn2 = false;}
                isSearching = false;
                syn1 = false;
            }
        else if(key != 8 && !((key>= 0 && key<=31) || (key>= 33 && key<=47) || (key>=58 && key<=64)|| (key>=91 && key<=96) || key>=123))
        {
            setstr+=key;
        }
    }

    void resetActivePos()
    {
        scbx.resetActivePos();
    }

    void OnMouseHover(int x,int y)
    {
        scbx.OnMouseHover(x,y);
    }

    bool mouseOverScrollBox(int x,int y)
    {
        if(x> ((10+scbx.getx1())*35) && x<((10+scbx.getwheelx1())*35) && y>((10-scbx.gety1())*32.5) && y<((10-scbx.gety2())*32.5) )
        {
            return true;
        }
        else
            return false;
    }

    void OnKeyPressed(unsigned char key,int x,int y)
    {
      if(isSearching)
      {
         setToString(keyword,key);
      }
      else if(syn1)
      {
         setToString(synonym1,key);
      }
      else if(syn2)
      {
         setToString(synonym2,key);
      }
    }

    void OnMouseDrag(int x,int y)
    {
        if(scbx.canScroll && wheelSelected)
        {
           if(y > ((10-(scbx.getwheely2() + scrollthreshold))*32.5) )
           {
               scbx.increaseWheelPos();
           }
           else if(y < ((10-(scbx.getwheely1() + scrollthreshold))*32.5))
           {
               scbx.decreaseWheelPos();
           }
        }
        if(fbscbx.canScroll && fbwheelSelected)
        {
           if(y > ((10-(fbscbx.getwheely2() + scrollthreshold))*32.5) )
           {
               fbscbx.increaseWheelPos();
           }
           else if(y < ((10-(fbscbx.getwheely1() + scrollthreshold))*32.5))
           {
               fbscbx.decreaseWheelPos();
           }
        }
    }

   void mouseFunc(int button,int state,int x,int y)
    {
        float xf, yf;
        xf = static_cast<float>(x)/35 - 10;
        yf = 10 - static_cast<float>(y)/32.5;
        //std::cout<<"\nxf: "<<xf<<"\tyf: "<<yf;
        wheelSelected = false;
        fbwheelSelected = false;
        if((button == GLUT_LEFT_BUTTON) &&(state == GLUT_DOWN) )
        {
            isSearching = false;
            syn1 = false;
            syn2 = false;
           if( xf< SB_X1 && xf > SB_X2 && yf < SB_Y1 && yf > SB_Y2)
           {
              isSearching = true;
           }
           else if( xf<(SB_X1+3.5) && xf> (SB_X1+1) && yf<SB_Y1 && yf > SB_Y2 )
           {
              searchKeyword();
           }
           else if(xf< SYNB1_X1 && xf>SYNB1_X2 && yf<SYNB1_Y1 && yf> SYNB1_Y2 )
           {
              syn1 = true;
           }
           else if(xf< SYNB2_X1 && xf > SYNB2_X2 && yf<SYNB2_Y1 && yf> SYNB2_Y2)
           {
              syn2 = true;
           }
           else if(xf< SSYNB_X1 && xf>SSYNB_X2 && yf<SSYNB_Y1 && yf> SSYNB_Y2)
           {
                setSyn();
           }
           else if(scbx.canScroll && (xf> scbx.getwheelx1() && xf<scbx.getwheelx2() && yf < scbx.getwheely1() && yf>scbx.getwheely2()) ) //ccordinate for scroll wheel is opposite to other textboxes
           {
              wheelSelected = true;
           }
           else if(fbscbx.canScroll && (xf> fbscbx.getwheelx1() && xf< fbscbx.getwheelx2() && yf < fbscbx.getwheely1() && yf>fbscbx.getwheely2()) ) //ccordinate for scroll wheel is opposite to other textboxes
           {
              fbwheelSelected = true;
           }
        }
        else if(button == 3 && state == GLUT_UP )
        {
            if(scbx.isMouseOver(x,y))
            {
                scbx.decreaseWheelPos();
                scbx.decActPos();
            }
            else if(fbscbx.isMouseOver(x,y))
            {
                fbscbx.decreaseWheelPos();
                fbscbx.decActPos();
            }
        }
        else if(button == 4 && state == GLUT_UP)
        {
            if(scbx.isMouseOver(x,y))
            {
                scbx.increaseWheelPos();
                scbx.incActPos();
            }
            else if(fbscbx.isMouseOver(x,y))
            {
                fbscbx.increaseWheelPos();
                fbscbx.incActPos();
            }
        }
    }

    void setUser(string username)
    {
        userName = username;
    }
    void createFileBox()
    {
        searchEngineDi.clear();
        fbscbx.setlines(searchEngineDi.getinfo(),searchEngineDi.getsize());
        DirectoryHandler userdhl;
        string userdir = string("./") + userName + string("/data");
        userdhl.setDirName(userdir.data());
        userdhl.setDirForUse();
        userdhl.displayfile();
        scbx.setlines(userdhl.dirDi.getinfo(),userdhl.dirDi.getsize());
        scbx.setActiveStatus(true);
    }
}
