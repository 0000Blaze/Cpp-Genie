#include <GL/glut.h>

namespace ErrorWindow
{
    int index;
    bool canMake = true;
    int mainWindowIndex;
    string errorText;
    void destroy()
    {
        glutDestroyWindow(index);
         canMake = true;
        glutSetWindow(mainWindowIndex);
    }
   void printText(std::string text,float x,float y,void* font)
    {
      glColor3f(0,0,0);
      glRasterPos2d(x,y);
      if(text.size()>40)
      {
          for (int i=0;i<40;i++)
            glutBitmapCharacter(font,text[i]);

          glRasterPos2d(x,y-0.13*1.5);

          for(int i=40;i<text.size();i++)
            glutBitmapCharacter(font,text[i]);
      }
      else
      {
        for (int i=0;i<text.size();i++)
            glutBitmapCharacter(font,text[i]);
      }

      glutPostRedisplay();
    }

   void render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        printText(errorText,-0.9,0.0,GLUT_BITMAP_8_BY_13);
        glutSwapBuffers();
    }

   void mouseFunc(int button,int state,int x,int y)
    {
        if((button == GLUT_LEFT_BUTTON) &&(state == GLUT_DOWN))
        {
            destroy();
        }
    }

    void create(const char* err,unsigned int posx,unsigned int posy)
    {
        index = glutCreateWindow("Error");
        errorText = err;
        glutSetWindow(index);
        glutPositionWindow(posx,posy);
        glutReshapeWindow(400,100);
        glutDisplayFunc(render);
        glClearColor(0.9, 0.9, 0.9, 1);
        glutMouseFunc(mouseFunc);
        canMake = false;
    }
}
