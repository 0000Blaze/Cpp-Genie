
class logIn{
private:
    std::string username,un;
    std::string password,pw;

public:

logIn(){  //default constructor
    username="";
    password="";
}

logIn(std::string usernamed,std::string passwordd):username(usernamed),password(passwordd){} //parametric constructor

bool IsLogedIn(){

    std::string path = std::string(".\\\\") + std::string(username) + std::string("\\\\password.txt");

    ifstream read((path).c_str());
        //read from file
    getline(read,un);
    getline(read,pw);

    if(username!="" && password!="")
    {
        if((un == username) && (pw == password))
        //check if username and password match
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;

    }
}
};
