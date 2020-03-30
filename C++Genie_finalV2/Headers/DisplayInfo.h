
class DisplayInfo
{
private:
    std::string* info;
    int size;
public:
    DisplayInfo()
    { size = 0;
    info = 0;}
    int getsize()
    { return size; }
    std::string* getinfo()
    { return info;}
    std::string getinfo(int i)
    {
        if(i<size)
        return info[i];
        else
            return std::string("");
    }
    void add(const char* str)
    {
       std::string* temp = new std::string[size+1];
       for(int i=0;i<size;i++)
          temp[i] = info[i];
        temp[size] = std::string(str);
        if(info != 0)
            delete[] info;
        info = temp;
        size++;
    }
    void add(std::string str)
    {
        add(str.data());
    }
    void clear()
    {
        delete[] info;
        info = 0;
        size = 0;
    }
    std::string get(int i)
    {
        if(i<size)
            return info[i];
        else
            return std::string("");
    }

    DisplayInfo& operator<<(const char* str)
    {
        add(str);
        return (*this);
    }
    DisplayInfo& operator<<(std::string str)
    {
        return (operator<<(str.data()));
    }
    ~DisplayInfo()
    {
        if(info != 0)
            delete[] info;
    }
};
