#include <GL/glut.h>
const float CHAR_WIDTH = 0.2667;

void printText(float x, float y,const char * text,void *font,float r,float g, float b)
{
    glColor3f(r,g,b);
    char *c;
    glRasterPos2f(x,y);
    char buf[100] = {0};
    sprintf(buf,text);
    for (c=buf;*c!='\0';c++)
    {
        glutBitmapCharacter(font,*c);
    }
    glColor3f(1.0,1.0,0.0);
    glutPostRedisplay();
}
 void printText(std::string tex,double x,double y,float max_x,void * font)
 {
      glColor3f(0,0,0);
      glRasterPos2d(x,y);
      int i = 0,a;
      float tsize = tex.size()* CHAR_WIDTH;
      if( tsize < (max_x-x))
        a =0;
      else
        a = (tex.size() - (int)((max_x - x)/CHAR_WIDTH));

      for (i=a;tex[i]!='\0';i++)
      {
        glutBitmapCharacter(font,tex[i]);
      }
      glColor3f(0,0,0);
      glutPostRedisplay();
 }
std::string viewTodoList(std::string uName,std::string task)
 {
        std::fstream file;
        std::string str;
        file.open(uName+"////Todo////"+task,std::ios::in);
        if(!file)
        {
            std::cerr<<"rfchm";
        }

            getline(file,str);
            getline(file,str);

        file.close();
        return str;

 }
 void glDrawP(float x, float y,float w,float h)
 {
     glBegin(GL_POLYGON);
     glVertex2f(x,y);
     glVertex2f(x+w,y);
     glVertex2f(x+w,y-h);
     glVertex2f(x,y-h);
     glEnd();
 }
void glDrawRecOutline(float a, float b,float c,float d)
{
    glBegin(GL_LINES);
        glVertex2f(a,b);   //LefttLine
        glVertex2f(a,d);

        glVertex2f(c,b);   //RightLine
        glVertex2f(c,d);

        glVertex2f(a,b);   //RLowerLine
        glVertex2f(c,b);

        glVertex2f(a,d);   //UpperLine
        glVertex2f(c,d);
    glEnd();
}
float toFloatX(int x)
{
    float tx;

    tx=static_cast<float>(x)/(70/2);

    if(tx<10)
    {
        tx=(10-tx)*(-1);
    }
    if(tx>10)
    {
        tx=(tx-10);
    }
    return tx;
}
float toFloatY(int y)
{
    float ty=static_cast<float>(y)/(65/2);
    if(ty<10)
    {
        ty=10-ty;
    }
    if(ty>10)
    {
        ty=(ty-10)*(-1);
    }
    return ty;
}
