#include <iostream>
using namespace std;

class Bike
{
public:
    friend class Bike_mgmt;
    int id;
    int type;
    int station;
    float price;
    int count;
    int s_time;
    bool available;


    Bike();
    Bike(int i, int t, int s, float p, int c);
    ~Bike();

    void setId(int Id);
    void setType(int Type);
    void setStation(int Station);
    void setPrice(float discount);
    void addCount();
    void setStime(int S_time);
    void setAvailable(bool f);

    const Bike &operator=(const Bike &b);
    bool operator>(Bike &b);
    bool operator<(Bike &b);
    friend ostream &operator<<(ostream &out, Bike &b);
};

Bike::Bike()
{
    id = -1;
    type = -1;
    station = -1;
    price = -1.0;
    count = -1;
    s_time = -1;
    available = true;
}

Bike::Bike(int i, int t, int s, float p, int c)
{
    id = i;
    type = t;
    station = s;
    price = p;
    count = c;
    s_time = -1;
    available = true;
}

Bike::~Bike() {}


void Bike::setId(int Id)
{
    id = Id;
}

void Bike::setType(int Type)
{
    type = Type;
}

void Bike::setStation(int Station)
{
    station = Station;
}

void Bike::setPrice(float discount)
{
    price -= discount;
}

void Bike::addCount()
{
    count++;
}

void Bike::setStime(int S_time)
{
    s_time = S_time;
}

void Bike::setAvailable(bool f)
{
    available = f;
}

const Bike &Bike::operator=(const Bike &b)
{
    id = b.id;
    type = b.type;
    station = b.station;
    price = b.price;
    count = b.count;
    s_time = b.s_time;

    return (*this);
}

bool Bike::operator>(Bike &b)
{
    if (price > b.price)
    {
        return true;
    }
    else if (price == b.price)
    {
        if (id < b.id)
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

bool Bike::operator<(Bike &b)
{
    if (price < b.price)
    {
        return true;
    }
    else if (price == b.price)
    {
        if (id > b.id)
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

ostream &operator<<(ostream &out, Bike &b)
{
    out << "Bike id = " << b.id << endl;
    out << "type = " << b.type << endl;
    out << "station = " << b.station << endl;
    out << "price = " << b.price << endl;
    out << "count = " << b.count << endl;
    out << "s_time = " << b.s_time << endl;
    // cout << endl;
}