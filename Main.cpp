#include <iostream>
#include "Graph.h"
using namespace std;
int main()
{
    try
    {
        Graph graph;
        graph.getFrom("Data.txt");
    
        cout << endl;
        cout << "Maximum flow is equal to " << graph.getMaxFlow();
        
    }
    catch (std::exception error)
    {
        cout << error.what();
    }
}
