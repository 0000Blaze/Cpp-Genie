#include "GlutUtility.h"

class TextBox
{
float CHAR_WIDTH;
float CHAR_HEIGHT;
private:
    float X1;  //left x-coord
    float Y1;  //top y-coord
    float X2;  //right x-coord
    float Y2;  //bottom y-coord
    Color idleColor;
    Color activeColor;
    int maxletter;
    bool isActive;
    string text;
    string label;
public:
    TextBox(float x1,float x2,float y1,float y2,float charwidth = 0.22857,float charheight = 0.61538,Color icolor = Color(1,1,1),Color acolor = Color(0.8,0.8,0.8))
    {
        X1 = x1; Y1 = y1; X2 = x2; Y2 = y2;
        CHAR_WIDTH = charwidth;
        CHAR_HEIGHT = charheight;
        idleColor = icolor;  activeColor = acolor;
        maxletter = (X1 - X2)/ CHAR_WIDTH;
        label = "";
    }

    bool getActive(){return isActive;}
    void setActive(bool act){ isActive = act;}
    void setText(const char* str){text = str;}
    string getText(){return text;}
    void setLabel(string lab){label = lab;}

    void display();
    bool mouseFunc(float x,float y);
    void onKeyPressed(unsigned int key);
    void printText(void* font);
    void makeBox(float x1,float x2,float y1,float y2,Color c);
};

    void TextBox::printText(void* font = GLUT_BITMAP_8_BY_13)
    {
      glColor3f(0,0,0);
      glRasterPos2d(X1+0.02,Y1-(Y1-Y2+CHAR_HEIGHT)/2);
      int i = 0;
      float tsize = text.size()* CHAR_WIDTH;
      if( tsize > (X2-X1))
        i = (text.size() - static_cast<unsigned int>((X2- X1)/CHAR_WIDTH));
      for (i;i<text.size();i++)
      {
        glutBitmapCharacter(font,text[i]);
      }
    }

    void TextBox::makeBox(float x1,float x2,float y1,float y2,Color c)
    {
        glBegin(GL_QUADS);
        glColor3f(c.r,c.g,c.b);
        glVertex2f(x2,y1);    //1st quadrant
        glVertex2f(x1,y1);    //2nd quadrant , x1 y1 is left-top corner
        glVertex2f(x1,y2);    //3rd quadrant
        glVertex2f(x2,y2);    //4th quadrant , x2 y2 is right-bottom corner
        glEnd();
        //std::cout<<"\nmadebox";
    }

    bool TextBox::mouseFunc(float xf, float yf)
    {
        if(xf >X1 && xf<X2 && yf<Y1 && yf>Y2)
            return true;
        else
            return false;
    }

    void TextBox::onKeyPressed(unsigned int key)
    {
        if(isActive)
        {
        if((key == 8)&&(text!=""))
        {
            text.pop_back();
        }
        else if(key != 8 && key !=13)
        {
            text += key;
        }
        }
    }

    void TextBox::display()
    {
        makeBox(X1,X2,Y1,Y2,idleColor);
        //std::cout<<"\nx1:"<<X1<<"\tx2"<<X2<<"\ty1"<<Y1<<"\ty2"<<Y2;
        if(isActive)
            makeBox(X1+0.01,X2-0.01,Y1-0.01,Y2+0.01,activeColor);
        if(text.size()>0)
            printText();
        if(label != "")
        {
            glColor3f(0,0,0);
            glRasterPos2d(X1+0.02,Y1+1.5*CHAR_HEIGHT);
            for (int i=0;i<label.size();i++)
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13,label[i]);

        }
    }







