#ifndef REQUEST_MGMT_H
#define REQUEST_MGMT_H

#include "User.h"
#include "MinHeap.h"
#include "Floyd.h"
#include "Bike_mgmt.h"
#include "IdString.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
#define MAX_STATION 1005
#define MAX_BIKE 10005

class User;

class Request_mgmt
{
    MinHeap<User> heap;

public:
    Request_mgmt();
    ~Request_mgmt();
    void Create(string file_path);
    User NextUser();
    void Pop();
    int getNum();
    void run_request(string file_path, string result_path);
    void verify(string result_path, string ans_path);
};

Request_mgmt::Request_mgmt() {}

Request_mgmt::~Request_mgmt() {}

void Request_mgmt::Create(string file_path)
{
    ifstream file;

    file.open(file_path + "/user.txt");
    if (file.is_open())
    {
        cout << "[user.txt] was opened properly ><!!" << endl;

        string line = "";
        stringstream ss;

        while (getline(file, line))
        {
            ss.str("");
            ss.clear();

            // [User ID] [bike types] [start time] [end time] [start station] [end station]
            //   U0         B0          1408          1420      S4              S3
            ss << line;

            string str = "";
            int ID, b_type, s_time, e_time, s_station, e_station;

            ss >> str;

            // set ID
            ID = stoi(str.substr(1, str.size() - 1));

            // get the bike type string
            string bike_type = "";
            ss >> bike_type;

            // define s_time & e_time
            ss >> str;
            s_time = stoi(str);
            ss >> str;
            e_time = stoi(str);

            // define s_station & e_station
            ss >> str;
            s_station = stoi(str.substr(1, str.size() - 1));
            ss >> str;
            e_station = stoi(str.substr(1, str.size() - 1));

            User u(ID, s_time, e_time, s_station, e_station);

            // create bike_type MinHeap
            stringstream bike_ss;
            bike_ss.str(bike_type);

            while (getline(bike_ss, str, ','))
            {
                b_type = stoi(str.substr(1, str.size() - 1));
                // cout << " b_type = " << b_type << endl;
                u.add_bike_type(b_type);
            }

            // Insert the User into the UserHeap
            heap.Insert(u);
        }

        file.close();
    }
    else
    {
        cout << "[user.txt] wasn't opened properly QQ!!" << endl;
    }
}

User Request_mgmt::NextUser()
{
    User u;
    u = heap.Top();
    return u;
}

void Request_mgmt::Pop()
{
    heap.Pop();
}

int Request_mgmt::getNum()
{
    return heap.getNum();
}

void Request_mgmt::run_request(string file_path, string result_path)
{
    // file stream
    ofstream file;

    // clean all files
    file.open(result_path + "/user_result.txt", ios_base::out);
    file.close();
    file.open(result_path + "/transfer_log.txt", ios_base::out);
    file.close();
    file.open(result_path + "/station_status.txt", ios_base::out);
    file.close();

    // Create all bikes
    Bike_mgmt bm;
    bm.Create(file_path);

    // Create minCost matrix
    int **minCost = Floyd(file_path);

    // manipulate all requests
    AVLTree<IdString> user_result;
    AVLTree<IdString> transfer_log;

    int cnt = 0;
    while (!heap.isEmpty())
    {
        User u = heap.Top();
        heap.Pop();

        int cost = minCost[u.s_station][u.e_station];
        bool accept = false;

        Bike rent_bike;
        if (u.s_time + cost < u.e_time)
        {
            // get available bike
            while (!u.bike_type->isEmpty()) // user may have several available bike types
            {
                int type = (*u.bike_type).Top();
                (*u.bike_type).Pop();

                Bike b = bm.getBike(u.s_station, type, u.s_time);
                if (b > rent_bike)
                {
                    rent_bike = b;
                }
            }

            if (rent_bike.type >= 0)
            {
                // request accept
                accept = true;
            }
        }

        if (accept) // accept the request
        {
            int revenue = rent_bike.price * cost;

            string uid = "U" + to_string(u.id);
            string acc = to_string(1);
            string bid = to_string(rent_bike.id);
            string st = to_string(u.s_time);
            string et = to_string(u.s_time + cost);
            string rev = to_string(revenue);
            string c = to_string(cost);
            string b_price = to_string(rent_bike.price);

            // Insert IdString into user_result
            string res = "";
            res = uid + " " + acc + " " + bid + " " + st + " " + et + " " + rev + "\n";
            string res_debug = res + " " + c + " " + b_price + "\n";
            IdString ur(res, u.id);
            user_result.root = user_result.Insert_with_ID_First(user_result.root, ur);

            // Insert IdString into transfer_log
            // Bike_Id  Start_Station   End_Station    Start_Time   End_Time    Bike_Rider(User_Id)
            string ss = "S" + to_string(u.s_station);
            string es = "S" + to_string(u.e_station);
            res = bid + " " + ss + " " + es + " " + st + " " + et + " " + uid + "\n";
            IdString tl(res, u.id);
            transfer_log.root = transfer_log.Insert_with_ID_First(transfer_log.root, tl);

            // update bike status
            bm.moveBike(rent_bike, u.s_station, u.e_station, u.s_time, cost);
        }
        else
        {
            // write in user_result.txt
            // Id   AcceptOrNot(1/0)   Bike_Id   Bike_Start_time    Bike_End_time   Revenue
            string res = "";
            res = "U" + to_string(u.id) + " 0 0 0 0 0\n";
            IdString ur(res, u.id);
            user_result.root = user_result.Insert_with_ID_First(user_result.root, ur);
        }
    }

    // write station_status.txt
    // station_id, bike_id, bike_type, rental_price, and rental_count
    // ascending order of station --> ascending order of bike_id
    for (int i = 0; i < MAX_STATION; i++)
    {
        AVLTree<Bike> *t = new AVLTree<Bike>;
        for (int j = 0; j < MAX_BIKE; j++)
        {
            if (!bm.manage[i][j].isEmpty())
            {
                bm.manage[i][j].Acending(bm.manage[i][j].root, t);
            }

            if (!bm.retire[i][j].isEmpty())
            {
                bm.retire[i][j].Acending(bm.retire[i][j].root, t);
            }
        }
        (*t).Write_Station_Status((*t).root, result_path, file);
    }

    // write user_result.txt
    string ur_path = result_path + "/user_result.txt";
    user_result.Write_File(user_result.root, ur_path, file);

    // write transfer_log.txt
    string tl_path = result_path + "/transfer_log.txt";
    transfer_log.Write_File(transfer_log.root, tl_path, file);
}

void Request_mgmt::verify(string res_path, string ans_path)
{
    bool flag = true;
    ifstream ans, res;
    string ans_line = "", res_line = "";

    // check [user_result.txt]
    ans.open(ans_path + "/user_result.txt");
    res.open(res_path + "/user_result.txt");

    cout << "#### Start checking USER_RESULT ####\n" << endl;
    if (ans.is_open() && res.is_open())
    {
        cout << "Two files open properly ><!!!\n" << endl;
        while (getline(ans, ans_line) && getline(res, res_line))
        {
            cout << "ans = " << ans_line << endl;
            cout << "res = " << res_line << endl;
            cout << endl;

            if (ans_line != res_line)
            {
                flag = false;
                cout << "[user_result.txt] isn't correct QQ!!" << endl;
                cout << "ans_line = " << ans_line << endl;
                cout << "res_line = " << res_line << endl;
                cout << endl;
            }
        }
        ans.close();
        res.close();
    }
    else
    {
        cout << "files don't open properly QQ!!\n"
             << endl;
    }

    // check [transfer_log.txt]
    ans.open(ans_path + "/transfer_log.txt");
    res.open(res_path + "/transfer_log.txt");

    cout << "#### Start checking TRANSFER_LOG ####\n" << endl;
    if (ans.is_open() && res.is_open())
    {
        cout << "Two files open properly ><!!!\n" << endl;
        ans_line = "", res_line = "";
        while (getline(ans, ans_line) && getline(res, res_line))
        {
            cout << "ans = " << ans_line << endl;
            cout << "res = " << res_line << endl;
            cout << endl;

            if (ans_line != res_line)
            {
                flag = false;
                cout << "[transfer_log.txt] isn't correct QQ!!" << endl;
                cout << "ans_line = " << ans_line << endl;
                cout << "res_line = " << res_line << endl;
                cout << endl;
            }
        }
        ans.close();
        res.close();
    }
    else
    {
        cout << "files don't open properly QQ!!\n"
             << endl;
    }

    // check [station_status.txt]
    ans.open(ans_path + "/station_status.txt");
    res.open(res_path + "/station_status.txt");

    cout << "#### Start checking STATION_STATUS ####\n" << endl;
    if (ans.is_open() && res.is_open())
    {
        cout << "Two files open properly ><!!!\n" << endl;
        ans_line = "", res_line = "";
        while (getline(ans, ans_line) && getline(res, res_line))
        {
            cout << "ans = " << ans_line << endl;
            cout << "res = " << res_line << endl;
            cout << endl;

            if (ans_line != res_line)
            {
                flag = false;
                cout << "[station_status.txt] isn't correct QQ!!" << endl;
                cout << "ans_line = " << ans_line << endl;
                cout << "res_line = " << res_line << endl;
                cout << endl;
            }
        }
        ans.close();
        res.close();
    }
    else
    {
        cout << "files don't open properly QQ!!\n\n"
             << endl;
    }

    if (flag)
    {
        cout << "\n\n*** All files are correct ^_^!! ***" << endl;
    }
}

#endif