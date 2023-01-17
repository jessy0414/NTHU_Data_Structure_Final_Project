#include "Bike.h"
#include "MinHeap.h"
#include "AVL_Tree.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
#define MAX_STATION 1005
#define MAX_BIKE 10005

class Bike_mgmt
{
friend class Request_mgmt;
    AVLTree<Bike>** manage;
    AVLTree<Bike>** retire;
    int cnt_limit;
    float discount;
    float *b_type_price;

public:
    Bike_mgmt();
    Bike_mgmt(int cl, int d);
    ~Bike_mgmt();
    int str_to_int(string str);
    float str_to_float(string str);
    void Create(string file_path);
    void Print();
    Bike getBike(int station, int type, int s_time);
    void moveBike(Bike b, int s_station, int e_station, int s_time, int cost);
};

Bike_mgmt::Bike_mgmt()
{
    cnt_limit = 0;
    discount = 0;
    b_type_price = new float[MAX_BIKE];
    for(int i=0; i<MAX_BIKE; i++)
    {
        b_type_price[i] = -1;
    }

    // store available bikes
    manage = new AVLTree<Bike>* [MAX_STATION];
    for(int i=0; i<MAX_STATION; i++)
    {
        manage[i] = new AVLTree<Bike> [MAX_BIKE];
    }

    // store retired bikes
    retire = new AVLTree<Bike>* [MAX_STATION];
    for(int i=0; i<MAX_STATION; i++)
    {
        retire[i] = new AVLTree<Bike> [MAX_BIKE];
    }
}

Bike_mgmt::Bike_mgmt(int cl, int d)
{
    cnt_limit = cl;
    discount = d;
    b_type_price = new float[MAX_BIKE];
    for(int i=0; i<MAX_BIKE; i++)
    {
        b_type_price[i] = -1;
    }

    // store available bikes
    manage = new AVLTree<Bike>* [MAX_STATION];
    for(int i=0; i<MAX_STATION; i++)
    {
        manage[i] = new AVLTree<Bike> [MAX_BIKE];
    }

    // store retired bikes
    retire = new AVLTree<Bike>* [MAX_STATION];
    for(int i=0; i<MAX_STATION; i++)
    {
        retire[i] = new AVLTree<Bike> [MAX_BIKE];
    }
}

Bike_mgmt::~Bike_mgmt()
{
}

int Bike_mgmt::str_to_int(string str)
{
    int num = 0;

    for (int i = 0; i < str.size(); i++)
    {
        int power = str.size() - 1 - i;
        int base = str[i] - '0';
        int offset = 1;

        while (power--)
        {
            offset *= 10;
        }

        offset *= base;
        num += offset;
    }

    return num;
}

float Bike_mgmt:: str_to_float(string str)
{
    stringstream ss(str);
    string s = "";

    float res = 0.0;

    // manipulate the int part and the float part separately
    int num = -1;
    while(getline(ss, s, '.'))
    {
        if(num < 0)
        {
            num = str_to_int(s);
            res += num;
        }
        else
        {
            for(int i=0; i<s.size(); i++)
            {
                int power = i + 1;
                int base = s[i] - '0';
                float offset = 1;

                while(power--)
                {
                    offset /= 10;
                }

                offset *= base;
                res += offset;

            }
        }
    }

    return res;
}

void Bike_mgmt::Create(string file_path)
{
    ifstream file;


    // *** create bike_info ***
    file.open(file_path + "/bike_info.txt");
    if (file.is_open())
    {
        cout << "[bike_info.txt] opened properly ><!!\n"
             << endl;

        stringstream ss;
        string line = "";
        string str = "";

        // define discount price
        getline(file, line);
        ss << line;
        ss >> str;
        discount = str_to_float(str);
        ss.str("");
        ss.clear();

        // define cnt_limit
        getline(file, line);
        ss << line;
        ss >> str;
        cnt_limit = str_to_int(str);

        // define b_type_price
        while (getline(file, line))
        {
            ss.str("");
            ss.clear();

            ss << line;

            // get b_type
            ss >> str;
            int bt = str_to_int(str.substr(1, str.size() - 1));

            // get init price
            ss >> str;
            float f = str_to_float(str);
            b_type_price[bt] = f;
        }

        // cout << "\nPrint [bike_info]" << endl;
        // Print();

        file.close();
    }
    else
    {
        cout << "[bike_info.txt] doesn't open properly QQ!!\n" << endl;
    }



    // *** create all bikes ***
    file.open(file_path + "/bike.txt");

    if (file.is_open())
    {
        cout << "[bike.txt] opened properly ><!!!\n" << endl;

        stringstream ss;
        string line;

        while(getline(file, line))
        {
            ss.str("");
            ss.clear();

            ss << line;
            string str;
            int Type, Id, Station, Count;
            float Price = 0.0;
            
            // define bike type
            ss >> str;
            Type = str_to_int(str.substr(1, str.size() - 1));

            // define bike id
            ss >> str;
            Id = str_to_int(str);

            // define locating station
            ss >> str;
            Station = str_to_int(str.substr(1, str.size() - 1));

            // define rental price
            ss >> str;
            Price = str_to_float(str);

            // define rental count
            ss >> str;
            Count = str_to_int(str);

            // insert bike
            Bike b(Id, Type, Station, Price, Count);
            manage[Station][Type].root = manage[Station][Type].Insert(manage[Station][Type].root, b);
        }

        // for(int i=0; i<MAX_STATION; i++)
        // {
        //     for(int j=0; j<MAX_BIKE; j++)
        //     {
        //         if(!manage[i][j].isEmpty())
        //         {
        //             cout << "*** station " << i << ", type " << j << " ***\n"; 
        //             manage[i][j].Decending(manage[i][j].root);
        //             cout << endl;
        //         }
        //     }
        // }
    }
    else
    {
        cout << "[bike.txt] didn't open properly QQ!!!\n" << endl;
    }
}

void Bike_mgmt:: Print()
{
    cout << "cnt_limit = " << cnt_limit << endl;
    cout << "discount = " << discount << endl;
    cout << "b_type_price = " << endl;
    for(int i=0; i<MAX_BIKE; i++)
    {
        if(b_type_price[i] > 0)
        {
            cout << "B" << i << ": " << b_type_price[i] << endl;
        }
    }

    cout << "\nPrint [bike.txt]" << endl;
    cout << endl;
}

Bike Bike_mgmt:: getBike(int station, int type, int s_time)
{
    Bike b = manage[station][type].getMaxBike(manage[station][type].root, s_time);
    return b;
}

void Bike_mgmt:: moveBike(Bike b, int s_station, int e_station, int s_time,  int cost)
{
    // delete the bike from the original station heap
    manage[s_station][b.type].root = manage[s_station][b.type].Delete(manage[s_station][b.type].root, b);

    // update bike status
    b.setStation(e_station);
    b.setPrice(discount);
    b.addCount();
    b.setStime(s_time + cost);
    if(b.count >= cnt_limit)
    {
        b.setAvailable(false);
    }

    // Insert the bike into the new station heap
    if(b.available)
    {
        manage[b.station][b.type].root = manage[b.station][b.type].Insert(manage[b.station][b.type].root, b);
    }
    else    // the bike is retired
    {
        retire[b.station][b.type].root = retire[b.station][b.type].Insert(retire[b.station][b.type].root, b);
    }
}