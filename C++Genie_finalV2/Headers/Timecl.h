#include <iostream>
#include <ctime>

class Showtime
{
private:
    float x_pos;
    float y_pos;
    float redn;
    float greenn;
    float bluen;
public:
    Showtime()
    {
        x_pos=100;
        y_pos=100;
    }
    Showtime(float x,float y,float r=1,float g=0,float b=0)
    {
        x_pos=x;
        y_pos=y;
        redn=r;
        greenn=g;
        bluen=b;
    }
    void displayClock(void *font)
    {
        time_t tim;
        tim =time(NULL);
        struct tm *local;
        local =localtime(&tim);
        int hr = local->tm_hour;
        if(hr>12)
            hr-=12;
        int hr1 =hr/10;
        int hr2 =hr%10;

        int mi = local->tm_min;

        int mi1 =mi/10;
        int mi2 =mi%10;

        int se = local->tm_sec;

        int se1=se/10;
        int se2=se%10;

        glColor3f(redn,greenn,bluen);

        glRasterPos2f(x_pos,y_pos);
        glutBitmapCharacter(font,static_cast<char>(hr1+48));

        glRasterPos2f(x_pos+1.35,y_pos);
        glutBitmapCharacter(font,static_cast<char>(hr2+48));

        glRasterPos2f(x_pos+2.72,y_pos);
        glutBitmapCharacter(font,static_cast<char>(58));

        glRasterPos2f(x_pos+3.5,y_pos);
        glutBitmapCharacter(font,static_cast<char>(mi1+48));

        glRasterPos2f(x_pos+4.75,y_pos);
        glutBitmapCharacter(font,static_cast<char>(mi2+48));

        glRasterPos2f(x_pos+5.8,y_pos);
        glutBitmapCharacter(font,static_cast<char>(58));

        glRasterPos2f(x_pos+6.5,y_pos);
        glutBitmapCharacter(font,static_cast<char>(se1+48));

        glRasterPos2f(x_pos+8.15,y_pos);
        glutBitmapCharacter(font,static_cast<char>(se2+48));

        glColor3f(1.0,1.0,0.0);

    }

    void displayCalendar(float calX,float calY,void *font)
    {
        time_t cal;
        cal =time(NULL);
        struct tm *local;
        local =localtime(&cal);

        int year =(local->tm_year)+1900;

        int y[2];
        y[2]=year%10;
        year/=10;
        y[1]=year%10;
        year/=10;
        y[0]=year%10;
        year/=10;

        int month =(local->tm_mon)+1;

        int m1=month/10;
        int m2=month%10;

        int day=local->tm_mday;

        int d1=day/10;
        int d2=day%10;

        glColor3f(redn,greenn,bluen);
        //YEAR
        glRasterPos2f(calX,calY);
        glutBitmapCharacter(font,static_cast<char>(y[2]+48));

        glRasterPos2f(calX-1.35,calY);
        glutBitmapCharacter(font,static_cast<char>(y[1]+48));

        glRasterPos2f(calX-2.7,calY);
        glutBitmapCharacter(font,static_cast<char>(y[0]+48));

        glRasterPos2f(calX-4,calY);
        glutBitmapCharacter(font,static_cast<char>(year+48));

        glRasterPos2f(calX+1.5,calY);
        glutBitmapCharacter(font,static_cast<char>(47));

        //MONTH
        glRasterPos2f(calX+2.5,calY);
        glutBitmapCharacter(font,static_cast<char>(m1+48));

        glRasterPos2f(calX+3.65,calY);
        glutBitmapCharacter(font,static_cast<char>(m2+48));

        glRasterPos2f(calX+5.05,calY);
        glutBitmapCharacter(font,static_cast<char>(47));

        //DAY
        glRasterPos2f(calX+5.65,calY);
        glutBitmapCharacter(font,static_cast<char>(d1+48));

        glRasterPos2f(calX+7.2,calY);
        glutBitmapCharacter(font,static_cast<char>(d2+48));

    }
};
