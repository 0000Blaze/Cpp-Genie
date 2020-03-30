class Color
{
public:
    float r;  //red
    float g;  //green
    float b;  //blue
    float a;  //alpha
    Color(float r1=1,float g1=1,float b1=1,float a1=1)
    { r = r1; g = g1; b = b1; a = a1;}
};

class Text: public Color
{
public:
    void* font;
    Text(){}
};
