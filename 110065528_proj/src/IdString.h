#include <iostream>
#include <string>
using namespace std;

class IdString
{
public:
    string str;
    int id;

    IdString();
    IdString(string s, int Id);
    ~IdString();
};

IdString:: IdString()
{
    str = "";
    id = -1;
}

IdString:: IdString(string s, int Id)
{
    str = s;
    id = Id;
}

IdString:: ~IdString() {}

