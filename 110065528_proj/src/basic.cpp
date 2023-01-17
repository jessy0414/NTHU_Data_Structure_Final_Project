#include "./nthu_bike.h"
#include "Request_mgmt.h"
#include <iostream>
#include <string>
using namespace std;

void basic(string selectedCase)
{
    // insert your code here
    cout << "start your basic version of data structure final from here!" << endl;

    // file path
    string file_path = "./testcases/" + selectedCase;
    string result_path = "./result/" + selectedCase;
    string ans_path = "./sorted_ans/" + selectedCase;
    ifstream file;

    // Create all requests (Users)
    Request_mgmt rm;
    rm.Create(file_path);

    // manage all requests and count the revenue
    rm.run_request(file_path, result_path);

    // verify the answer
    rm.verify(result_path, ans_path);

}
