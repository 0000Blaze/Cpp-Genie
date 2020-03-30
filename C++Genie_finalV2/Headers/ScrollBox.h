#include "TextBox.h"

class ScrollBox
{
const float CHAR_HEIGHT = 0.6153846;   //height of each char is 13 pixels so (13+7)/300 = 0.066667
const float CHAR_WIDTH = 0.22857;

private:
    float X1;
    float Y1;
    float X2;
    float Y2;
    int activeline;
    int activeStatusPos;
    float wheelheight;
    int maxlines;
    string* lines;
    int wheelpos;
    int nooflines;
    int maxletter;
    int maxWheelpos;
     float wheel_X1;
     float wheel_X2;
     float wheel_Y1;
     float wheel_Y2;
     bool showActiveStatus = false;
public:
     bool canScroll = false;
     bool mouseOver = false;

    ScrollBox(float x1,float y1,float x2,float y2)
    {
        X1 = x1; Y1 = y1; X2 = x2; Y2 = y2;
       maxletter = (X2-1 - X1)/ CHAR_WIDTH;
       maxlines = (int)((Y1 - Y2)/ CHAR_HEIGHT);
       activeline = -1;
    }
    string getActiveLine()
    {
        if(activeline >=0 && activeline < nooflines)
        {
            return lines[activeline];
        }
        else
            return string("");

    }
    void setActiveStatus(bool acst)
    {
        showActiveStatus = acst;
    }
    void setlines(string* inlin,int stringsize)
    {
        if(nooflines>0)
            delete[] lines;
        nooflines = stringsize;
        for(int i=0;i<stringsize;i++)
         {
             if(inlin[i].size()>maxletter)
             {
                nooflines++;
             }

         }
         lines = new string[nooflines];
         for(int i=0 ,sc=0;sc<stringsize;i++,sc++)
         {
             if(inlin[sc].size()<=maxletter)
             {
                lines[i] = inlin[sc];
             }
             else     //breaking strings to remove overflow
             {
                 lines[i] = inlin[sc].substr(0,maxletter);
                 lines[i+1] = inlin[sc].substr(maxletter,inlin[sc].size()-1);
                 i++;
             }
         }
        if(nooflines > maxlines)
        {
            wheelheight = (Y1 - Y2)/((nooflines-maxlines) + 1);
            wheelpos = 0;
            canScroll = true;
            maxWheelpos = nooflines-maxlines;
        }
        else
        {
            wheelheight = 0;
            wheelpos = -1;
            canScroll = false;
        }
        wheel_X1 = X2 - 0.05*10;
        wheel_X2 = X2;
        wheel_Y1 = Y1-wheelheight*wheelpos;
        wheel_Y2 = wheel_Y1-wheelheight;
    }

    float getwheelx1()
    {return wheel_X1;}
     float getwheelx2()
    {return wheel_X2;}
     float getwheely1()
    {return wheel_Y1;}
     float getwheely2()
    {return wheel_Y2;}

    float getx1()
    {return X1;}
     float getx2()
    {return X2;}
     float gety1()
    {return Y1;}
     float gety2()
    {return Y2;}

    void increaseWheelPos()
    {
        if(wheelpos < maxWheelpos)
        {
        wheelpos++;
        wheel_Y1 = Y1-wheelheight*wheelpos;
        wheel_Y2 = wheel_Y1-wheelheight;
        }
    }
    void decreaseWheelPos()
    {
        if(wheelpos > 0)
        {
        wheelpos--;
        wheel_Y1 = Y1-wheelheight*wheelpos;
        wheel_Y2 = wheel_Y1-wheelheight;
        }
    }
    void incActPos()
    {
        if(activeline<nooflines-1)
            activeline++;
    }
    void decActPos()
    {
        if(activeline>0)
            activeline--;
    }
    void resetActivePos()
    {
        activeline = -1;
    }

    void mousePosToLineNo(int x,int y)
    {
        float xi,yi;
        yi = 1*10-(float)(y/32.5);
        xi = (float)(x/35)-10;
        if((Y1-Y2-CHAR_HEIGHT)>(Y1-yi))
        {
            activeline = (int)((Y1-yi)/CHAR_HEIGHT);
            if((activeline >= nooflines))
                activeline = -1;
            if(wheelpos!=-1)
                activeline += wheelpos;
        }
    }

    void OnMouseHover(int x,int y)
    {
        mousePosToLineNo(x,y);
    }

    bool isMouseOver(int x,int y)
    {
        float xf,yf;
        xf = static_cast<float>(x)/35 - 10;
        yf = 10 - static_cast<float>(y)/32.5;
        if(xf>X1 && xf<X2 && yf<Y1 && yf > Y2 )
            return true;
        else
            return false;
    }

    bool isMouseOver(float xf,float yf)
    {
        if(xf>X1 && xf<X2 && yf<Y1 && yf > Y2 )
            return true;
        else
            return false;
    }

 private:
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

    void printText(std::string text,float x,float y,float max_x)
    {
      glColor3f(0,0,0);
      glRasterPos2d(x,y);
        for (int i = 0;i < text.size() ;i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13,text[i]);
        }
      glutPostRedisplay();
    }

    void printLines()
    {
        if(nooflines<=maxlines)
            for(int i=0;i<nooflines;i++)
                printText(lines[i],X1+0.05*10,Y1-i*CHAR_HEIGHT-0.05*10,X2-0.05*10);
        else
        {  for(int i=0;i<maxlines;i++)
                printText(lines[i+wheelpos],X1+0.05*10,Y1-i*CHAR_HEIGHT-0.05*10,X2-0.05*10);
        }
    }
  public:
    void display()
    {
        makeBox(X1,X2,Y1+0.05,Y2,Color(1,1,1));  //scroll box
        makeBox(X2-0.05*10,X2,Y1+0.05,Y2,Color(0.5,0.5,0.5));  //scrollwheel placement box
        if(activeline>-1 && showActiveStatus)
        {
            if(wheelpos>-1)
                activeStatusPos = activeline - wheelpos;
            else
                activeStatusPos = activeline;
            makeBox(X1+0.1,X2-0.6,Y1-activeStatusPos*CHAR_HEIGHT,(Y1-activeStatusPos*CHAR_HEIGHT)-CHAR_HEIGHT,Color(0.145,0.8745,0.06667));  //activeline status box
        }
        if(canScroll)
            makeBox(wheel_X1,wheel_X2,wheel_Y1,wheel_Y2,Color(0.2,0.2,0.2));  //scroll wheel
        if(nooflines>0)
            printLines();
    }
    ~ScrollBox()
    {
        if(nooflines>0)
            delete[] lines;
    }
};


