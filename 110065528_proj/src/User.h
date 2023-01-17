#ifndef USER_H
#define USER_H

#include "MinHeap.h"
#include <iostream>
#include <sstream>
using namespace std;


class User
{
friend class Request_mgmt;
friend class MinHeap<int>;
    int id;
    int s_time, e_time;
    int s_station, e_station;
    MinHeap<int>* bike_type;

public:
    User();
    User(int ID, int S_time, int E_time, int S_station, int E_station);
    ~User();
    void add_bike_type(int n);
    int get_bike_type();
    bool operator<(const User & u);
    bool operator>(const User &u);
    const User& operator=(const User &u);
    friend ostream &operator<<(ostream &out, User &u);
    void Print_btype();
};


User:: User()
{
    id = 0;
    s_time = 0;
    e_time = 0;
    s_station = 0;
    e_station = 0;
    bike_type = new MinHeap<int>();
}

User:: User(int ID, int S_time, int E_time, int S_station, int E_station)
{
    id = ID;
    s_time = S_time;
    e_time = E_time;
    s_station = S_station;
    e_station = E_station;
    bike_type = new MinHeap<int>();
}

User:: ~User() {}

void User:: add_bike_type(int n)
{
    (*bike_type).Insert(n);
}

int User:: get_bike_type()
{
    int n;
    n = (*bike_type).Top();
    (*bike_type).Pop();
    return n;
}

bool User:: operator<(const User &u)
{
    if(s_time < u.s_time)
    {
        return true;
    }
    else if(s_time == u.s_time)
    {
        if(id < u.id)
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

bool User:: operator>(const User &u)
{
    if(s_time > u.s_time)
    {
        return true;
    }
    else if(s_time == u.s_time)
    {
        if(id > u.id)
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

const User& User:: operator=(const User &u)
{
    id = u.id;
    s_time = u.s_time;
    e_time = u.e_time;
    s_station = u.s_station;
    e_station = u.e_station;
    bike_type = u.bike_type;

    return (*this);
}

void User:: Print_btype()
{
    cout << "\nPrint_b_type()\n" << endl;
    (*bike_type).Print((*bike_type).getRoot());
}

ostream &operator<<(ostream &out, User &u)
{
    out << "User ID: " << u.id << endl;

    out << "Bike type: ";
    // out << u.Print_btype();
    out << endl;

    out << "s_time: " << u.s_time << endl;
    out << "e_time: " << u.e_time << endl;

    out << "s_station: " << u.s_station << endl;
    out << "e_station: " << u.e_station << endl;

    out << endl;
}


#endif