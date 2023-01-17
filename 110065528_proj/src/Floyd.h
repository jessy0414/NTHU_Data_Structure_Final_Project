#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
#define MAX_STATION 1005
#define INF 9999999

int** Floyd(string file_path)
{
    // Create minCost matrix
    int** minCost = new int* [MAX_STATION];
    for(int i=0; i<MAX_STATION; i++)
    {
        minCost[i] = new int [MAX_STATION];
    }

    // set init value
    for(int i=0; i<MAX_STATION; i++)
    {
        for(int j=0; j<MAX_STATION; j++)
        {
            minCost[i][j] = INF;
        }
    }

    // read file and construct A0
    ifstream file;

    file.open(file_path + "/map.txt");
    if(file.is_open())
    {
        cout << "The file is open properly ><!!" << endl;

        string line;
        stringstream ss;
        while(getline(file, line))
        {
            // reset the stringstream
            ss.str("");
            ss.clear();

            // set new string
            ss << line;

            // add the path to the graph
            // 1. get the edge cost and its two end vertices
            string n1, n2, cost;
            ss >> n1 >> n2 >> cost;
            
            // 2. change those strings into integers
            int s1, s2, c = 0;
            s1 = stoi(n1.substr(1, n1.size() - 1));
            s2 = stoi(n2.substr(1, n2.size() - 1));
            for(int i=cost.size() - 1; i>=0; i--)
            {
                int power = (cost.size() - 1) - i;
                int base = cost[i] - '0';
                
                int level = 1;
                while(power--)
                {
                    level *= 10;
                }

                c += base * level;
            }
            
            minCost[s1][s2] = c;
            minCost[s2][s1] = c;

        }

        file.close();
        line = "";
    }
    else
    {
        cout << "Floyd can't open map.txt properly!!QQ\n";
    }

    // Floyd-Warshall algo
    for(int k=0; k<MAX_STATION; k++)
    {
        for(int i=0; i<MAX_STATION; i++)
        {
            for(int j=0; j<MAX_STATION; j++)
            {
                if(minCost[i][j] > minCost[i][k] + minCost[k][j])
                {
                    minCost[i][j] = minCost[i][k] + minCost[k][j];
                }
            }
        }
    }

    return minCost;

}